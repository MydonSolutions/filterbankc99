/* printer.h
 */
#ifndef FILTERBANK_C99_PRINTER_H
#define FILTERBANK_C99_PRINTER_H

#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

// #define PRINTER_VERBOSE

#ifdef __cplusplus
extern "C" {
#endif

/* Call this to log an error message */
void filterbank_print_error(const char *name, const char *msg, ...);

/* Call this to log an warning message */
void filterbank_print_warn(const char *name, const char *msg, ...);

/* Call this to log an informational message */
void filterbank_print_info(const char *name, const char *msg, ...);

/* Call this to log verbose message */
void filterbank_print_verbose(const char *name, const char *msg, ...);

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_C99_PRINTER_H
