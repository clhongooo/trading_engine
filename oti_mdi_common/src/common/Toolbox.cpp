#include "Toolbox.h"
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <boost/algorithm/string/regex.hpp>
using namespace atu;
using namespace std;

std::string Toolbox::itos(int num) {
	std::stringstream out;
	out << num;
	return out.str();
}

std::string Toolbox::ftos(double num) {
	std::stringstream out;
	out << num;
	return out.str();
}
double Toolbox::meanInList(vector<double> &mylist)
{
	double sum=0;
	for (unsigned int i=0;i<mylist.size();i++) {
		sum+=mylist[i];
	}
	if (mylist.size()>0) {
		return sum/(double)(mylist.size());
	} else {
		return 0.0;
	}
}

bool Toolbox::isFileExist(const char *filename) {
	ifstream ifile(filename);
	return ifile.is_open();
}
int Toolbox::getNextDay(int org_date, int nDayForward){
	int year = org_date / 10000;
	int month = (org_date % 10000) / 100;
	int day = org_date % 100;
	struct std::tm a = {0,0,0,day,month-1,year-1900};
    time_t rawtime=mktime(&a);
  	struct tm * timeinfo;
	rawtime+=nDayForward*60*60*24; // Add one day
	timeinfo = localtime ( &rawtime );
	return (timeinfo->tm_year+1900) * 10000 + (timeinfo->tm_mon+1) * 100 + timeinfo->tm_mday;
}
int Toolbox::getNextDay(int org_date){
	return getNextDay(org_date,1);
}
int Toolbox::getPrevDay(int org_date){
	return getNextDay(org_date,-1);
}
double Toolbox::average(double array[], int from, int size) {
	double aver = 0;
	for (int i = from; i < size; i++) {
		aver += array[i];
	}
	return aver / size;
}

double Toolbox::average(int array[], int from, int size) {
	double aver = 0;
	int endindex = from + size;
	for (int i = from; i < endindex; i++) {
		aver += array[i];
	}
	return aver / size;
}
void Toolbox::FindAndReplace( std::string& tInput, std::string tFind, std::string tReplace )
{
        size_t uPos = 0;
        size_t uFindLen = tFind.length();
        size_t uReplaceLen = tReplace.length();
        if( uFindLen == 0 )
        {
                return;
        }

	for( ;(uPos = tInput.find( tFind, uPos )) != std::string::npos; )
        {
                tInput.replace( uPos, uFindLen, tReplace );
                uPos += uReplaceLen;
        }
}
string Toolbox::tolower(std::string &str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}
int Toolbox::timetosec(int a)
{
	int hr=a/10000;
	int min=(a%10000)/100;
	int sec=(a%10000)%100;
	return hr*3600+min*60+sec;
}
int Toolbox::sectotime(int a)
{
	int hr=a/3600;
	int min=(a%3600)/60;
	int sec=(a%60);
	return hr*10000+min*100+sec;
}
int Toolbox::sum_time(int t,int b)
{
	int a=timetosec(t);
	return sectotime(a+b);
}
int Toolbox::diff_time(int a,int b)
{
	return timetosec(a)-timetosec(b);
}
string Toolbox::timeStamp()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [200];
/*
  struct timeb tp;
  time ( &rawtime );
  ftime(&tp);
*/
//  double curmsec=m_Timer->getTimeInMS();
  time(&rawtime);
  //rawtime=(long long)(curmsec/1000.0);
  timeinfo = localtime ( &rawtime );
  strftime (buffer,200,"%Y%m%d%H%M%S",timeinfo);
  char outtime[1000];
//  double justms=(curmsec-floor(curmsec/1000.0)*1000.0)*100;

  sprintf(outtime,"%s%05ld",buffer,(long)0);
  return string(outtime);
}
string Toolbox::timeStamp_with_underscore()
{
  time_t rawtime;
  struct tm * timeinfo;
  char buffer [200];
/*
  struct timeb tp;
  time ( &rawtime );
  ftime(&tp);
*/
//  double curmsec=m_Timer->getTimeInMS();
  time(&rawtime);
  //rawtime=(long long)(curmsec/1000.0);
  timeinfo = localtime ( &rawtime );
  strftime (buffer,200,"%Y%m%d_%H%M%S",timeinfo);
  char outtime[1000];
//  double justms=(curmsec-floor(curmsec/1000.0)*1000.0)*100;

  sprintf(outtime,"%s_%06ld",buffer,(long)0);
  return string(outtime);
}
string Toolbox::timeStamp_full()
{
	timespec tS;
	clock_gettime(CLOCK_REALTIME, (timespec*) (&(tS)));
	long long cur = (long long) tS.tv_nsec;
	double curmsec = ((double) cur / double(1000 * 1000)) + (double) tS.tv_sec * (double) 1000;
	return timeStamp_full(curmsec);
}
string Toolbox::timeStamp_full(double curms) {
	char TimeStampStr[200];
	struct tm * timeinfo;
	struct tm timeinfo2;
	char buffer[200];
	//struct timeb tp;
	time_t tvsec=(time_t)(curms/1000);
	timeinfo = localtime_r((time_t*) (&(tvsec)), &timeinfo2);
	strftime(buffer, 200, "%Y%m%d_%H%M%S", &timeinfo2);
	double ms=curms*1000-tvsec*1000*1000;
	sprintf((char*) (TimeStampStr), "%s_%06d", buffer, (int)(ms));
	return TimeStampStr;
}
string Toolbox::timeStamp_gmtfull(double curms) {
	char TimeStampStr[200];
	struct tm * timeinfo;
	struct tm timeinfo2;
	char buffer[200];
	//struct timeb tp;
	time_t tvsec=(time_t)(curms/1000);
	timeinfo = gmtime_r((time_t*) (&(tvsec)), &timeinfo2);
	strftime(buffer, 200, "%Y%m%d_%H%M%S", &timeinfo2);
	double ms=curms*1000-tvsec*1000*1000;
	sprintf((char*) (TimeStampStr), "%s_%06d", buffer, (int)(ms));
	return TimeStampStr;
}
boost::posix_time::ptime  Toolbox::timeStamp_to_ptime(string timestamp){
	boost::algorithm::replace_last(timestamp,"_",".");
	boost::posix_time::time_input_facet* facet = new boost::posix_time::time_input_facet("%Y%m%d_%H%M%S%f");
	std::stringstream ss;
	ss.imbue(std::locale(std::locale(), facet));
	ss << timestamp;
	boost::posix_time::ptime pt = boost::posix_time::microsec_clock::local_time();
	ss >> pt;
	return pt;
}
double Toolbox::timeStamp_toMS(string timestamp) {
	if (timestamp.length()==22) {
		int year=atoi(timestamp.substr(0,4).c_str());
		int month=atoi(timestamp.substr(4,2).c_str());
		int day=atoi(timestamp.substr(6,2).c_str());
		int hour=atoi(timestamp.substr(9,2).c_str());
		int min=atoi(timestamp.substr(11,2).c_str());
		int sec=atoi(timestamp.substr(13,2).c_str());
		int msec=atoi(timestamp.substr(16,6).c_str());
		struct tm* timeinfo;
		time_t rawtime;
		time(&rawtime);
		timeinfo=localtime(&rawtime);

		timeinfo->tm_year=year-1900;
		timeinfo->tm_mon=month-1;
		timeinfo->tm_mday=day;
		timeinfo->tm_hour=hour;
		timeinfo->tm_min=min;
		timeinfo->tm_sec=sec;

		double epotime=mktime(timeinfo)*1000;
		epotime=epotime+msec/1000000;
		return epotime;
	}
}
void Toolbox::printStringVector(vector<string> &string_vec) {
	unsigned int size = string_vec.size();
	for (unsigned int i = 0; i < size; i++) {
		cout << __FUNCTION__ << ",size=" << size << ",i=" << i << ",value=" << string_vec[i] << endl;
	}
}

bool Toolbox::isNotAlNumSpaceUnderScoreMinus(char c)
{
    return !(isalpha(c) || isdigit(c) || (c == ' ') || (c == '_') || (c == '.') || (c == '-'));
}
bool Toolbox::isStringValid(const std::string &str)
{
    return find_if(str.begin(), str.end(), isNotAlNumSpaceUnderScoreMinus) == str.end();
}

bool Toolbox::stringToKeyValuePair(map<string,string> &keyvaluepairdict, string instr)
{
	vector<string> attribvec;
	boost::split(attribvec, instr, boost::is_any_of(";"));
	for (int i = 0; i < attribvec.size(); i++) {
		vector<string> keyvaluepairvec;
		boost::split(keyvaluepairvec, attribvec[i], boost::is_any_of("="));
		if (keyvaluepairvec.size()!=2) {
			return false;
		} else {
			if (keyvaluepairvec[0].length()==0 || keyvaluepairvec[1].length()==0) {
				return false;
			} else {
				keyvaluepairdict[keyvaluepairvec[0]]=keyvaluepairvec[1];
			}
		}
	}
	return true;
}
void Toolbox::split(vector<string> &result,std::string& in ,std::string delem) {
	boost::algorithm::split_regex( result, in, boost::regex( delem ) ) ;

}

