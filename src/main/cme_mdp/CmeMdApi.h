#pragma once

#include <cstdio>
#include <set>
#include <map>
#include <string>
#include <vector>

#include <MessageHeader.h>

class CmeMdApi {
  public:
    virtual ~CmeMdApi() = default;

    virtual void Init() = 0;
    void SubscribeMarketData(const std::vector<std::string>& instruments);
    void LoadInstrumentsConfig(const std::map<int, std::string>& config_map);

    struct level {
      int64_t price;
      int size;
      int norder;
    };

    struct OrderBook {
      uint64_t transact_time;
      int seqno;
      int secid;
      struct level bid[10], ask[10];
      char instid[16];
    };

  protected:
    struct binhdr {
      uint32_t msg_seqno;
      uint64_t sending_time;
    };

    virtual void OnRefreshBook(const OrderBook *ob) = 0;

    void OnRefreshBook(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnInstrumentDefinitionOption(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshVolume(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshTrade(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshTradeSummary(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshDailyStatistics(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshSessionStatistics(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshLimitsBanding(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshSecurityDefinitionFuture(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnRefreshSecurityDefinitionSpread(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnQuoteRequest(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnSecurityStatus(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnChannelReset(mktdata::MessageHeader& hdr, char *buf, int len);
    void OnSnapshotFullRefresh(mktdata::MessageHeader& hdr, char *buf, int len);

    void Disseminate(char *buf, int len);
    void ResetBooks();

    const int ver = 6;
    FILE *fp;
    std::set<std::string> subscribe;
    std::map<std::string, OrderBook> obmap;
    std::map<int, std::string> instmap;

    std::string TransferTimestamp(uint64_t micro_sec);
};
