#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>

#include "filterbankc99/filterbank_utils.h"

// Conversion utilities

double filterbank_ddd_to_dms(double ddd)
{
  int sign = ddd < 0 ? -1 : +1;
  ddd = fabs(ddd);
  double mm = 60*fmod(ddd, 1.0);
  double ss = 60*fmod(mm,  1.0);
  ddd = floor(ddd);
  mm  = floor(mm);
  return sign * (10000*ddd + 100*mm + ss);
}

double filterbank_dms_to_ddd(double dms)
{
  double dd;
  double mm;
  double ss;

  int sign = dms < 0 ? -1 : +1;
  dms = fabs(dms);

  dd = floor(dms / 10000);
  dms -= 10000 * dd;
  mm = floor(dms / 100);
  ss = dms - 100 * mm;
  dd += mm/60.0 + ss/3600.0;
  return sign * dd;
}

double rawspec_raw_dmsstr_to_d(char * dmsstr)
{
  int sign = 1;
  double d = 0.0;

  char * saveptr;
  char * tok;

  if(dmsstr[0] == '-') {
    sign = -1;
    dmsstr++;
  } else if(dmsstr[0] == '+') {
    dmsstr++;
  }

  tok = strtok_r(dmsstr, ":", &saveptr);
  if(tok) {
    // Degrees (or hours)
    d = strtod(tok, NULL);

    tok = strtok_r(NULL, ":", &saveptr);
    if(tok) {
      // Minutes
      d += strtod(tok, NULL) / 60.0;

      tok = strtok_r(NULL, ":", &saveptr);
      if(tok) {
        // Seconds
        d += strtod(tok, NULL) / 3600.0;
        tok = strtok_r(NULL, ":", &saveptr);
      }
    }
  } else {
    d = strtod(dmsstr, NULL);
  }

  return sign * d;
}
