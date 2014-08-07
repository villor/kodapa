#include <stdio.h>
#include <stdarg.h>

#include "log.h"

#define K_LOG_FP "kodapa.log"

void klog(const char *fmt, ...) {
	va_list arg;
	va_start(arg, fmt);

	FILE *fp;
	fp = fopen(K_LOG_FP, "a+");

	fprintf(fp, "LOG: ");
	vfprintf(fp, fmt, arg);
	fprintf(fp, "\n");

	fclose(fp);
	va_end(arg);
}
