#ifndef _FILTERBANK_C99_UTILS_H_
#define _FILTERBANK_C99_UTILS_H_

#include <stdlib.h>

// Conversion utilities

double filterbank_ddd_to_dms(double ddd);
double filterbank_dms_to_ddd(double dms);
double filterbank_dmsstr_to_ddd(char * dmsstr);

#endif // _FILTERBANK_C99_UTILS_H_