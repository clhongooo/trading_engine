#ifndef STDSTREAMLOGGER_H
#define STDSTREAMLOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

using namespace boost;

class StdStreamLogger {
  public:
    enum LogLevel {EMERGENCY=0, ALERT, CRITICAL, ERROR, WARNING, NOTICE, INFO, DEBUG};

    static boost::shared_ptr<StdStreamLogger> Instance();
    ~StdStreamLogger() {}

    void SetLogLevel(LogLevel);
    LogLevel GetLogLevel() const;
    void Write(const LogLevel, const char *, ...);

    static LogLevel m_LogLevel;

  private:
    StdStreamLogger(){};
    StdStreamLogger(StdStreamLogger const&){};
    StdStreamLogger& operator=(StdStreamLogger const&){};
    static boost::weak_ptr<StdStreamLogger> m_pInstance;
};

#endif
