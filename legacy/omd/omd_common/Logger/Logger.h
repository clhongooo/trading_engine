//**************************************************
//  Author:      Sunny Yan
//  Created On:  Fri Apr 4 14:07:27 HKT 2014
//  Description:
//
//**************************************************

#ifndef LOGGER_H_
#define LOGGER_H_

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

// --------------------------------------------------
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread.hpp>
// --------------------------------------------------
#include <pantheios/pantheios.hpp>
#include <pantheios/inserters/integer.hpp>
#include <pantheios/inserters/pointer.hpp>
#include "pantheios/frontends/stock.h"
#include <pantheios/inserters/args.hpp>

// --------------------------------------------------
// Output To File
// Headers for main()
#include <pantheios/backends/bec.file.h> 

// Headers for implicit linking
#include <pantheios/implicit_link/core.h>
//#include <pantheios/implicit_link/fe.simple.h> //doesn't allow the severity level to change
//#include <pantheios/frontend.h> //allow the severity level to change
#include <pantheios/implicit_link/be.file.h>
// --------------------------------------------------

using namespace std;



// --------------------------------------------------
// Pantheois - Begin
// --------------------------------------------------
extern const PAN_CHAR_T PANTHEIOS_FE_PROCESS_IDENTITY[];

namespace
{ static int s_severityCeiling = pantheios::emergency; }

PANTHEIOS_CALL(PAN_CHAR_T const*) pantheios_fe_getProcessIdentity(void*);
PANTHEIOS_CALL(int) pantheios_fe_init(void *, void **ptoken);
PANTHEIOS_CALL(void) pantheios_fe_uninit(void *);
PANTHEIOS_CALL(int) pantheios_fe_isSeverityLogged(void*,int severity,int);
// --------------------------------------------------
// Pantheois - End
// --------------------------------------------------


class Logger {
  public:
    enum LogLevel {EMERGENCY, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG};

    static boost::shared_ptr<Logger> Instance();
    ~Logger();

    void SetLogLevel(LogLevel);
    void SetEnableWriteMutex(bool);
    void SetPath(const char*);
    void Write(LogLevel, const char *, ...);
    void Write2(LogLevel, char *);
    bool DoWeNeedToDo(LogLevel);

  private:
    Logger();
    Logger(Logger const&){};
    Logger& operator=(Logger const&){};
    static boost::weak_ptr<Logger> m_pInstance;
    static const char * m_LogPath;
    boost::shared_mutex m_SharedMutex;
    bool m_bEnableWriteMutex;
};

#endif
