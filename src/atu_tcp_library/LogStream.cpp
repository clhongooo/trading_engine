#include "LogStream.h"
#include <iostream>
using namespace atu;
using namespace std;

LogStream::LogStream() {
#ifdef _ATUDEBUG_
	cout << "LogStream default log level created..." << endl;
#endif // End of _ATUDEBUG_
	LogStream(1);
}
LogStream::LogStream(int loglevel) {
#ifdef _ATUDEBUG_
	cout << "LogStream with log level " << loglevel << " created..." << endl;
#endif // End of _ATUDEBUG_
	m_loglevel = loglevel;
}
LogStream::~LogStream() {
#ifdef _ATUDEBUG_
	cout << "LogStream destroyed..." << endl;
#endif // End of _ATUDEBUG_
}
std::basic_string<char, std::char_traits<char>, std::allocator<char> > LogStream::str() {
	return m_oss.str();
}
