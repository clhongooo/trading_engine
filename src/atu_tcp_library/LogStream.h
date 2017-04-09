#ifndef _LOGSTREAM_H_
#define _LOGSTREAM_H_
#include <sstream>
using namespace std;


/* Example of usage
LogStream los(5);
los << "Hello" << " ";
los << setprecision(2);
los << setw(5);
los << setfill('0');
los << 1.555222 << " " << endl;
cout << los.str() << " ok" << endl;
*/

namespace atu {
	class LogStream {
		public:
			LogStream();
			LogStream(int);
			virtual ~LogStream();
			virtual std::basic_string<char, std::char_traits<char>, std::allocator<char> > str();
			template <class T> LogStream& operator<< (const T& v) {
				this->m_oss << v;
				return (*this);
			}
			LogStream& operator <<( std::ostream& (*funcptr)(std::ostream &) ){
				funcptr(m_oss);
		        return *this;
		    }

// Unknown if needed below
// Ref: http://stackoverflow.com/questions/11851104/error-no-match-for-operator-when-working-with-stdstring
/*
			LogStream& operator <<( std::ios_base& (*manip)(std::ios_base&) ) {
		        manip( m_oss);
		        return *this;
		    }
*/

		private:
			int m_loglevel;
			ostringstream m_oss;
	};

}
#endif // End of _LOGSTREAM_H_
