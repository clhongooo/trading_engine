#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>

#include "common.h"

CDECLS_BEGIN;

void err_sys(const char *fmt, ...);
void err_msg(const char *fmt, ...);
void err_exit(const char *fmt, ...);
void OnLoginFail(const char *category, const char *user, int err, const char *msg);

CDECLS_END;

#endif
