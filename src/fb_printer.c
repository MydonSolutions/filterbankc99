/* printer.c
 */
#include "filterbankc99/filterbank_printer.h"

/* For now just put it all to stderr.
 * Maybe do something clever like a stack in the future?
 */
void filterbank_print_error(const char *name, const char *msg, ...) {
    fprintf(stderr, "FilterbankC99 Error (%s)", name);
    if(msg) {
        va_list ap;
        va_start(ap, msg);
        fprintf(stderr, ": ");
        vfprintf(stderr, msg, ap);
        va_end(ap);
    }
    if(errno) {
        fprintf(stderr, " [%s]", strerror(errno));
    }
    fprintf(stderr, "\n");
    fflush(stderr);
}

void filterbank_print_warn(const char *name, const char *msg, ...) {
    fprintf(stderr, "FilterbankC99 Warning (%s)", name);
    if(msg) {
        va_list ap;
        va_start(ap, msg);
        fprintf(stderr, ": ");
        vfprintf(stderr, msg, ap);
        va_end(ap);
    }
    fprintf(stderr, "\n");
    fflush(stderr);
}

void filterbank_print_info(const char *name, const char *msg, ...) {
    fprintf(stdout, "FilterbankC99 Info (%s)", name);
    if(msg) {
        va_list ap;
        va_start(ap, msg);
        fprintf(stdout, ": ");
        vfprintf(stdout, msg, ap);
        va_end(ap);
    }
    fprintf(stdout, "\n");
    fflush(stdout);
}

void filterbank_print_verbose(const char *name, const char *msg, ...) {
#ifdef PRINTER_VERBOSE
    fprintf(stdout, "FilterbankC99 Verbage (%s)", name);
    if(msg) {
        va_list ap;
        va_start(ap, msg);
        fprintf(stdout, ": ");
        vfprintf(stdout, msg, ap);
        va_end(ap);
    }
    fprintf(stdout, "\n");
    fflush(stdout);
#endif
}