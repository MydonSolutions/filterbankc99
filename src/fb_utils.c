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

double filterbank_dmsstr_to_ddd(char * dmsstr)
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

int filterbank_telescope_id(const char *telescope_name)
{
	int id = -1;

  // This mapping from copied from aliases.c from the sigproc code base
  if (strcasecmp(telescope_name,"FAKE")==0)
    id=0;
  else if (strcasecmp(telescope_name,"ARECIBO")==0)
    id=1;
  else if (strcasecmp(telescope_name,"OOTY")==0)
    id=2;
  else if (strcasecmp(telescope_name,"NANCAY")==0)
    id=3;
  else if (strcasecmp(telescope_name,"PARKES")==0)
    id=4;
  else if (strcasecmp(telescope_name,"JODRELL")==0)
    id=5;
  else if (strcasecmp(telescope_name,"GBT")==0)
    id=6;
  else if (strcasecmp(telescope_name,"GMRT")==0)
    id=7;
  else if (strcasecmp(telescope_name,"EFFELSBERG")==0)
    id=8;
  else if (strcasecmp(telescope_name,"140FT")==0)
    id=9;
  // ATA used to be 10, but the SRT is now 10 and (at leasst in bl_sigproc) ATA
  // is 9.  The "140FT" telescope used to be 9 so we still map from that name
  // to 9 even though bl_sigproc will display it as "ATA".  AFAIK, the 140FT is
  // not producing GUPPI RAW files, so there is little chance that this will
  // ever cause any problems (but it would still be nice to reconcile
  // telescope_id across all sigproc-derived libraries).
  else if (strcasecmp(telescope_name,"ATA")==0)
    id=9;
  else if (strcasecmp(telescope_name,"SRT")==0)
    id=10;
  else if (strcasecmp(telescope_name,"LEUSCHNER")==0)
    id=11;
  else if (strcasecmp(telescope_name,"MEERKAT")==0)
    id=64;

	return id;
}
