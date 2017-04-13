#include <cstdio>
#include <stdint.h>
#include <cmath>
#include <queue>
#include <fstream>
#include <vector>

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "error.h"
#include "CmeMdApi.h"

using namespace std;

queue<CmeMdApi::OrderBook> orderbookq;
pthread_spinlock_t lock;

int date_now();

class CmeMd : public CmeMdApi {
  public:
    CmeMd(const string& src_host, const string& cfg_file);
    void Init();

  protected:
    void OnRefreshBook(const OrderBook *ob);
    const string src_host;
    const string cfg_file;
    int src_sock;
    FILE *md_file;
};

  CmeMd::CmeMd(const string& src_host, const string& cfg_file) 
: src_host(src_host), cfg_file(cfg_file)
{
  int date = date_now();
  char file_name[64];
  snprintf(file_name, sizeof(file_name), "cme-md-%d.txt", date);
  md_file = fopen(file_name, "w+");
}

  void
CmeMd::Init()
{
  fstream config_file;
  config_file.open(cfg_file.c_str(), fstream::in);
  map<int, string> cfg_map;
  if (config_file.good()) {
    char buf[512];
    while(!config_file.eof()) {
      config_file.getline(buf, 512);
      char *secid = strtok(buf, ",");
      char *instid = strtok(NULL, "\0");
      if (secid != NULL && instid != NULL) {
        cfg_map.insert(pair<int, string>(atoi(secid), instid));
      }
    }
  }
  LoadInstrumentsConfig(cfg_map);

  char *mcip = strtok((char *)strrchr(src_host.c_str(), '/') + 1, ":");
  int port = strtol(strtok(NULL, "|"), NULL, 10);
  char *localip = strtok(NULL, "\0");

  if ( (src_sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    err_sys("open socket error");

  int reuse = 1;

  if (setsockopt(src_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) == -1)
    err_sys("set SO_REUSEADDR error");

  int opt = 1;

  if (ioctl(src_sock, FIONBIO, &opt) == -1)
    err_sys("socket fail to set FIONBIO");

  struct sockaddr_in cli = {0};

  cli.sin_family = AF_INET;
  cli.sin_port = htons(port);
  cli.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(src_sock, (struct sockaddr*)&cli, sizeof(cli)) == -1)
    err_sys("bind error");

  struct ip_mreq group;

  group.imr_multiaddr.s_addr = inet_addr(mcip);
  group.imr_interface.s_addr = inet_addr(localip);

  if (setsockopt(src_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < -1)
    err_sys("add multicast group error");

  int n;
  char buf[4096];
  printf("reading...\n");
  while (1) {
    n = read(src_sock, buf, sizeof(buf));

    if (errno == EAGAIN) {
      errno = 0;
      continue;
    }

    Disseminate(buf, n);
  }

  err_sys("fail");
}

  void
CmeMd::OnRefreshBook(const OrderBook *ob)
{
  // pthread_spin_lock(&lock);
  // orderbookq.push(*ob);
  // pthread_spin_unlock(&lock);

  printf("Orderbook: %d %lu,%d", ob->secid, ob->transact_time, ob->seqno);

  for (int i = 0; i < 10; ++i) {
    printf(",[%d]", i + 1);
    printf(",%ld,%d,%d", ob->bid[i].price, ob->bid[i].size, ob->bid[i].norder);
    printf(",%ld,%d,%d", ob->ask[i].price, ob->ask[i].size, ob->ask[i].norder);
    fprintf(md_file, ",[%d]", i + 1);
    fprintf(md_file, ",%ld,%d,%d", ob->bid[i].price, ob->bid[i].size, ob->bid[i].norder);
    fprintf(md_file, ",%ld,%d,%d", ob->ask[i].price, ob->ask[i].size, ob->ask[i].norder);
  }

  printf("\n");
  fprintf(md_file, "\n");
}

  void *
writer(void *args)
{
  while (true) {
    pthread_spin_lock(&lock);

    if (orderbookq.empty()) {
      pthread_spin_unlock(&lock);
      sleep(1);
      continue;
    }

    CmeMdApi::OrderBook ob = orderbookq.front();

    orderbookq.pop();
    pthread_spin_unlock(&lock);

    printf("%lu,%d,%d,%ld,%d,%ld,%d\n", ob.transact_time, ob.seqno, ob.secid, ob.bid[0].price, ob.bid[0].size, ob.ask[0].price, ob.ask[0].size);
  }

  return NULL;
}

  int
main(int argc, char *argv[])
{
  if (argc < 4)
    err_exit("usage: %s src_multicast config_file inst_id1 inst_id2 ...", argv[0]);

  CmeMd mdapi(argv[1], argv[2]);
  vector<string> instid;

  for (int i = 3; i < argc; ++i)
    instid.push_back(argv[i]);

  pthread_t tid;
  pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE);
  pthread_create(&tid, NULL, writer, NULL);

  mdapi.SubscribeMarketData(instid);
  mdapi.Init();

  return 0;
}

int date_now() 
{
  time_t rawtime;
  struct tm *timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  int date = (1900+timeinfo->tm_year)*10000 +
    (1+timeinfo->tm_mon)*100 + timeinfo->tm_mday;
  return date;
}
