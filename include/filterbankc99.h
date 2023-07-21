// Routines for reading and writing filterbank headers from/to files and memory
// buffers.  A filterbank header consists of ASCII keywords that are preceeded
// by a 4 byte integer specifying the ASCII keyword length.  Each keyword is
// followed by 0 or more values, but usually just 1.  Each keyword has a
// specific type of data that are expected.  For maximum compatibility with
// historic implementions and currently dominant byte endianness, this library
// outputs integer and double values in little endian format.
//
// The format of a filterbank header can be describled by this ASCII-gram:
//
//     IIIIHEADER_START
//     IIIImachine_idIIII
//     IIIItelescope_idIIII
//     IIIIsrc_rajDDDDDDDD
//     IIIIsrc_dejDDDDDDDD
//     IIIIaz_startDDDDDDDD
//     IIIIza_startDDDDDDDD
//     IIIIdata_typeIIII
//     IIIIfch1DDDDDDDD
//     IIIIfoffDDDDDDDD
//     IIIInchansIIII
//     IIIInbeamsIIII
//     IIIIibeamIIII
//     IIIInbitsIIII
//     IIIItstartDDDDDDDD
//     IIIItsampDDDDDDDD
//     IIIInifsIIII
//     IIIIsource_nameIIII[C...]
//     IIIIrawdatafileIIII[C...]
//     IIIIHEADER_END
//
// Where IIII is a 4-byte integer, DDDDDDDD is an 8 byte double precision
// floating point number, and [C...] is a variable length ASCII string (not NUL
// terminated).  Additional keywords exist in the SIGPROC spec, but these are
// the keywords currently supported by the routines in this file.
//
// Here is a sample output from the SIGPROC `header` program which reads a
// filterbank header and outputs a human-readable version:
//
// Data file                        : blc34_guppi_57856_70053_DIAG_DR21_0034.gpuspec.0000.fil
// Header size (bytes)              : 384
// Data size (bytes)                : 805306368
// Data type                        : filterbank (topocentric)
// Telescope                        : GBT
// Datataking Machine               : ?????
// Source Name                      : DIAG_DR21
// Source RA (J2000)                : 20:39:07.4
// Source DEC (J2000)               : +42:24:24.5
// Frequency of channel 1 (MHz)     : 4626.464842353016138
// Channel bandwidth      (MHz)     : -0.000002793967724
// Number of channels               : 67108864
// Number of beams                  : 1
// Beam number                      : 1
// Time stamp of first sample (MJD) : 57856.810798611114
// Gregorian date (YYYY/MM/DD)      : 2017/04/13
// Sample time (us)                 : 18253611.00800
// Number of samples                : 3
// Observation length (seconds)     : 54.8
// Number of bits per sample        : 32
// Number of IFs                    : 1

#ifndef _FILTERBANK_C99_H_
#define _FILTERBANK_C99_H_

#include <stdint.h>
#include <sys/types.h>

#include "filterbankc99/filterbank_header.h"
#include "filterbankc99/filterbank_utils.h"
#include "filterbankc99/filterbank_write_utils.h"
#include "filterbankc99/filterbank_read_utils.h"

#endif // _FILTERBANK_C99_H_
