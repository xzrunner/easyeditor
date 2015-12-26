#include "log.h"
#include <stdarg.h>

void 
log_init(struct log *log, FILE *f) {
	log->f = f;
}

void 
log_printf(struct log *log, const char * format, ...) {
	va_list ap;
	va_start(ap, format);
	vfprintf(log->f, format, ap);
	va_end(ap);
	fflush(log->f);
}