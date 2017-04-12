#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include <sys/file.h>

#include "error.h"

void
err_sys(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fprintf(stderr, ": %s\n", strerror(errno));

	exit(-1);
}

void
err_msg(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fprintf(stderr, "\n");
}

void
err_exit(const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);

	fprintf(stderr, "\n");

	exit(-1);
}

void
OnLoginFail(const char *category, const char *user, int err, const char *msg)
{
	FILE *fp;

	if ( (fp = fopen("sys.status", "a")) == NULL)
		err_sys("fail to open sys.status");

	int fd = fileno(fp);

	flock(fd, LOCK_EX);

	fprintf(fp, "%s %s %d %s\n", category, user, err, msg);

	flock(fd, LOCK_UN);
	fclose(fp);
}
