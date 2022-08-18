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

#ifdef __cplusplus
extern "C" {
#endif

// Conversion utilities

double filterbank_ddd_to_dms(double ddd);
double filterbank_dms_to_ddd(double dms);

// Write utilities

ssize_t filterbank_fd_write_int(int fd, int32_t i);
void * filterbank_buf_write_int(void * buf, int32_t i);

ssize_t filterbank_fd_write_double(int fd, double d);
void * filterbank_buf_write_double(void * buf, double d);

ssize_t filterbank_fd_write_angle(int fd, double d);
void * filterbank_buf_write_angle(void * buf, double d);

ssize_t filterbank_fd_write_string(int fd, const char * c);
void * filterbank_buf_write_string(void * buf, const char * c);

// Read utilities

int32_t filterbank_fd_read_int(int fd, int32_t * i);
void * filterbank_buf_read_int(void * buf, int32_t * i);

double filterbank_fd_read_double(int fd, double * d);
void * filterbank_buf_read_double(void * buf, double * d);

double filterbank_fd_read_angle(int fd, double * d);
void * filterbank_buf_read_angle(void * buf, double * d);

ssize_t filterbank_fd_read_string(int fd, char * c, int32_t * n);
void * filterbank_buf_peek_string(void * buf, char ** c, int32_t * n);
void * filterbank_buf_read_string(void * buf, char * c, int32_t * n);

// Header functions

ssize_t filterbank_fd_write_padded_header(int fd, const filterbank_header_t * hdr, int32_t minlen);
ssize_t filterbank_fd_write_header(int fd, const filterbank_header_t * hdr);
void * filterbank_buf_write_padded_header(void * buf, const filterbank_header_t * hdr, int32_t minlen);
void * filterbank_buf_write_header(void * buf, const filterbank_header_t * hdr);

ssize_t filterbank_fd_read_header(int fd, filterbank_header_t * hdr, size_t * hdr_len);
void * filterbank_buf_read_header(void * buf, filterbank_header_t * hdr, size_t * hdr_len);

int filterbank_telescope_id(const char *telescope_name);
#ifdef __cplusplus
}
#endif

#endif // _FILTERBANK_C99_H_
