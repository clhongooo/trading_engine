#ifndef _TOOLBOX_H_
#define _TOOLBOX_H_
#include "PCH.h"
#include "Constants.h"
#include <boost/date_time.hpp>
using namespace std;
namespace atu {
	struct delete_ptr
	{
		template <class T>
		void operator()(T* p)
		{
			delete p;
		}
	};

	enum {
		e_bid, e_ask
	};
	enum {
		e_sell = -1, e_hold = 0, e_buy = 1
	};
	class Toolbox {
		public:
			Toolbox();
			~Toolbox();
			static std::string itos(int);
			static std::string ftos(double);
			static bool isFileExist(const char*);
			static int getNextDay(int,int);
			static int getNextDay(int); // Format = YYYYMMDD, e.g. 20130131, same format for return
			static int getPrevDay(int);
//			template <class T>
//			static double average(vector<T>[], int, int);
			static double average(double[],int,int);
			static double average(int[],int,int);
			static void FindAndReplace( std::string& tInput, std::string tFind, std::string tReplace );
			static string tolower(std::string &str);
			static int timetosec(int);
			static int sectotime(int);
			static int sum_time(int a,int b);
			static int diff_time(int a,int b);
			static std::string timeStamp();
			static std::string timeStamp_with_underscore();
			static boost::posix_time::ptime  timeStamp_to_ptime(string);
			static void printStringVector(std::vector<std::string>&);
			static double meanInList(vector<double> &mylist);
			static bool isNotAlNumSpaceUnderScoreMinus(char c);
			static bool isStringValid(const std::string &str);
			static bool stringToKeyValuePair(map<string,string> &keyvaluepairdict, string instr);
			static string timeStamp_full();
			static string timeStamp_full(double curms);
			static string timeStamp_gmtfull(double curms);
			static double timeStamp_toMS(string m);
			static void split(vector<string> & result, std::string& in ,std::string delem);
	};
}
#endif // End of _TOOLBOX_H_
