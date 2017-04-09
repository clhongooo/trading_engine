#ifndef _ATU_ERRORMSGSTRUCT_H_
#define _ATU_ERRORMSGSTRUCT_H_

//using namespace atu;
#include "PCH.h"
#include "Constants.h"
#include <boost/function.hpp>
using namespace std;
namespace atu {

class ATU_ErrorMsgStruct {
public:
	string m_timestamp;
	string m_errormsg;
	string m_source;
	ATU_ErrorMsgStruct():
		m_timestamp(""),
		m_errormsg(""),
		m_source(""){}
};
typedef boost::function<bool(ATU_ErrorMsgStruct*)> NotifyErrorMsgFunction;
typedef boost::function<bool(ATU_ErrorMsgStruct*)> ATU_errorfeed_CallBackFunc;
}

#endif //_ATU_ERRORMSGSTRUCT_H_
