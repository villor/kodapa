#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include "log.h"

#define K_LOG_FP "kodapa.log"

void fputs_timedate(FILE *fp)
{
	char s[25];
	time_t t;
	t = time(NULL);
	struct tm *p = localtime(&t);
	strftime(s, 25, "%c", p);
	fputs(s, fp);
}

void klog(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);

	FILE *fp;
	fp = fopen(K_LOG_FP, "a+");

	fputs_timedate(fp);
	fputs(": ", fp);
	vfprintf(fp, fmt, arg);
	fputs("\n", fp);

	fclose(fp);
	va_end(arg);
}
