// Routines for reading and writing filterbank headers from/to files and memory
// buffers.  See fbutils.h for more details.

#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>

#include "filterbankc99.h"

// Write utilities

ssize_t filterbank_fd_write_int(int fd, int32_t i)
{
  i = htole32(i);
  return write(fd, &i, sizeof(int32_t));
}

void * filterbank_buf_write_int(void * buf, int32_t i)
{
  i = htole32(i);
  *((int32_t *)buf) = i;
  return buf + sizeof(int32_t);
}

ssize_t filterbank_fd_write_double(int fd, double d)
{
  uint64_t i = htole64(*(uint64_t *)&d);
  return write(fd, &i, sizeof(uint64_t));
}

void * filterbank_buf_write_double(void * buf, double d)
{
  uint64_t i = htole64(*(uint64_t *)&d);
  *((int64_t *)buf) = i;
  return buf + sizeof(int64_t);
}

ssize_t filterbank_fd_write_angle(int fd, double d)
{
  return filterbank_fd_write_double(fd, filterbank_ddd_to_dms(d));
}

void * filterbank_buf_write_angle(void * buf, double d)
{
  return filterbank_buf_write_double(buf, filterbank_ddd_to_dms(d));
}

// Only write max 80 characters
ssize_t filterbank_fd_write_string(int fd, const char * c)
{
  int32_t len = strnlen(c, 80);
  filterbank_fd_write_int(fd, len);
  return write(fd, c, len) + sizeof(int32_t);
}

// Only write max 80 characters
void * filterbank_buf_write_string(void * buf, const char * c)
{
  int32_t len = strnlen(c, 80);
  buf = filterbank_buf_write_int(buf, len);
  memcpy(buf, c, len);
  return buf + len;
}

// Read utilities

int32_t filterbank_fd_read_int(int fd, int32_t * i)
{
  int ii;
  read(fd, &ii, sizeof(int32_t));
  ii = le32toh(ii);
  if(i) {
    *i = ii;
  }
  return ii;
}

void * filterbank_buf_read_int(void * buf, int32_t * i)
{
  if(i) {
    *i = le32toh(*(int32_t *)buf);
  }
  return buf + sizeof(int32_t);
}

double filterbank_fd_read_double(int fd, double * d)
{
  uint64_t ii;
  read(fd, &ii, sizeof(int64_t));
  ii = le64toh(ii);
  if(d) {
    *d = *(double *)&ii;
  }
  return *(double *)&ii;
}

void * filterbank_buf_read_double(void * buf, double * d)
{
  uint64_t ii;
  if(d) {
    ii = *(uint64_t *)buf;
    ii = le64toh(ii);
    *d = *(double *)&ii;
  }
  return buf + sizeof(int64_t);
}

double filterbank_fd_read_angle(int fd, double * d)
{
  double dd = filterbank_fd_read_double(fd, NULL);
  dd = filterbank_dms_to_ddd(dd);
  if(d) {
    *d = dd;
  }
  return dd;
}

void * filterbank_buf_read_angle(void * buf, double * d)
{
  buf = filterbank_buf_read_double(buf, d);
  if(d) {
    *d = filterbank_dms_to_ddd(*d);
  }
  return buf;
}

// Only read max *n characters
ssize_t filterbank_fd_read_string(int fd, char * c, int32_t * n)
{
  int32_t total_len, read_len;
  if(!c || !n) {
    return -1;
  }
  // Read length
  read(fd, &total_len, sizeof(int32_t));
  read_len = total_len = le32toh(total_len);
  // If length to read is greater than size of buffer
  if(read_len > *n) {
    // Truncate read length
    read_len = *n;
  }
  // Read string
  read_len = read(fd, c, read_len);
  if(read_len == -1) {
    return -1;
  }
  // If space remains in buffer, NUL terminate
  if(read_len < *n) {
    c[read_len] = '\0';
  }
  // If read was truncated, seek past rest of string
  if(read_len < total_len) {
    lseek(fd, total_len - read_len, SEEK_CUR);
  }
  // Store and return length read
  *n = read_len;
  return read_len;
}

// Sets *c to point to start of NOT-nul-terminated string in buf.  Sets *n to
// length of string.  Returns buf+sizeof(int)+length_of_string unless c or n in
// NULL in which case it returns buf.
void * filterbank_buf_peek_string(void * buf, char ** c, int32_t * n)
{
  if(!c || !n) {
    return buf;
  }
  *c = filterbank_buf_read_int(buf, n);
  return *c + *n;
}

// Only read max *n characters
void * filterbank_buf_read_string(void * buf, char * c, int32_t * n)
{
  char * bufstr;
  int32_t len;
  if(!c || !n) {
    return buf;
  }
  // Peek at string
  buf = filterbank_buf_peek_string(buf, &bufstr, &len);

  // If length to read is greater than size of buffer
  if(len > *n) {
    // Truncate read length
    len = *n;
  }
  // Copy string
  memcpy(c, bufstr, len);
  // If space remains in buffer, NUL terminate
  if(len < *n) {
    c[len] = '\0';
  }
  // Store length
  *n = len;

  return buf;
}

// Header functions

// Writes a filterbank header padded as close to minlen as possible.  Padding
// is performed by outputting multiple "rawdatafile" header entries with dummy
// values before the final "rawdatafile" header entry and real value.  Because
// the minimum string length is 1, the minimum padding that can be applied is
// 4+11+4+1 == 20 bytes for a one byte dummy value for the "rawdatafile"
// keyword.  The maximum padding value that can be applied by one "rawdatafile"
// header entry is 4+11+4+79 == 98 bytes.  An arbitrary amount of padding is
// achieved by padding 4+11+4+60 == 79 bytes at a time (i.e. as a sequence of
// "rawdatafile" keywords each with 60 byte dummy values) so long as the
// remaining pad length is greater than 98.  This ensures that the final
// padding entry will be at least 20 characters but no more than 98.
// TODO Add return value checking
ssize_t filterbank_fd_write_padded_header(int fd, const filterbank_header_t * hdr, int32_t minlen)
{
  ssize_t n;
  int32_t padlen;
  //                 0        1         2         3         4
  //                 1234567890123456789012345678901234567890
  char padstr[80] = "                                        "
                    "                                       ";

  n  = filterbank_fd_write_string(fd, "HEADER_START");
  n += filterbank_fd_write_string(fd, "machine_id");
  n += filterbank_fd_write_int(   fd, hdr->machine_id);
  n += filterbank_fd_write_string(fd, "telescope_id");
  n += filterbank_fd_write_int(   fd, hdr->telescope_id);
  n += filterbank_fd_write_string(fd, "src_raj");
  n += filterbank_fd_write_angle( fd, hdr->src_raj);
  n += filterbank_fd_write_string(fd, "src_dej");
  n += filterbank_fd_write_angle( fd, hdr->src_dej);
  n += filterbank_fd_write_string(fd, "az_start");
  n += filterbank_fd_write_double(fd, hdr->az_start);
  n += filterbank_fd_write_string(fd, "za_start");
  n += filterbank_fd_write_double(fd, hdr->za_start);
  n += filterbank_fd_write_string(fd, "data_type");
  n += filterbank_fd_write_int(   fd, hdr->data_type);
  n += filterbank_fd_write_string(fd, "fch1");
  n += filterbank_fd_write_double(fd, hdr->fch1);
  n += filterbank_fd_write_string(fd, "foff");
  n += filterbank_fd_write_double(fd, hdr->foff);
  n += filterbank_fd_write_string(fd, "nchans");
  n += filterbank_fd_write_int(   fd, hdr->nchans);
  n += filterbank_fd_write_string(fd, "nbeams");
  n += filterbank_fd_write_int(   fd, hdr->nbeams);
  n += filterbank_fd_write_string(fd, "ibeam");
  n += filterbank_fd_write_int(   fd, hdr->ibeam);
  n += filterbank_fd_write_string(fd, "nbits");
  n += filterbank_fd_write_int(   fd, hdr->nbits);
  n += filterbank_fd_write_string(fd, "tstart");
  n += filterbank_fd_write_double(fd, hdr->tstart);
  n += filterbank_fd_write_string(fd, "tsamp");
  n += filterbank_fd_write_double(fd, hdr->tsamp);
  n += filterbank_fd_write_string(fd, "nifs");
  n += filterbank_fd_write_int(   fd, hdr->nifs);
  if(hdr->barycentric) {
    n += filterbank_fd_write_string(fd, "barycentric");
    n += filterbank_fd_write_int(   fd, hdr->barycentric);
  }
  if(hdr->pulsarcentric) {
    n += filterbank_fd_write_string(fd, "pulsarcentric");
    n += filterbank_fd_write_int(   fd, hdr->pulsarcentric);
  }
  n += filterbank_fd_write_string(fd, "source_name");
  n += filterbank_fd_write_string(fd, hdr->source_name);

  // Make strlen(padstr) be 79-(4+11+4)
  padstr[79-19] = '\0';
  padlen = minlen - n
         - (2*sizeof(int32_t)+strlen("rawdatafile")+strlen(hdr->rawdatafile))
         - (  sizeof(int32_t)+strlen("HEADER_END"));
  while(padlen > 98) {
    n += filterbank_fd_write_string(fd, "rawdatafile");
    n += filterbank_fd_write_string(fd, padstr);
    padlen -= 79;
  }
  if(padlen > 19) {
    padstr[79-19] = ' ';
    padstr[padlen-19] = '\0';
    n += filterbank_fd_write_string(fd, "rawdatafile");
    n += filterbank_fd_write_string(fd, padstr);
  }

  n += filterbank_fd_write_string(fd, "rawdatafile");
  n += filterbank_fd_write_string(fd, hdr->rawdatafile);
  n += filterbank_fd_write_string(fd, "HEADER_END");

  return n;
}

ssize_t filterbank_fd_write_header(int fd, const filterbank_header_t * hdr)
{
  return filterbank_fd_write_padded_header(fd, hdr, 0);
}

// Writes a filterbank header padded as close to minlen as possible.  See
// comments for filterbank_fd_write_padded_header() for more details.
void * filterbank_buf_write_padded_header(void * buf, const filterbank_header_t * hdr, int32_t minlen)
{
  int32_t padlen;
  //                 0        1         2         3         4
  //                 1234567890123456789012345678901234567890
  char padstr[80] = "                                        "
                    "                                       ";
  void * buf0 = buf;
  buf = filterbank_buf_write_string(buf, "HEADER_START");
  buf = filterbank_buf_write_string(buf, "machine_id");
  buf = filterbank_buf_write_int(   buf, hdr->machine_id);
  buf = filterbank_buf_write_string(buf, "telescope_id");
  buf = filterbank_buf_write_int(   buf, hdr->telescope_id);
  buf = filterbank_buf_write_string(buf, "src_raj");
  buf = filterbank_buf_write_angle( buf, hdr->src_raj);
  buf = filterbank_buf_write_string(buf, "src_dej");
  buf = filterbank_buf_write_angle( buf, hdr->src_dej);
  buf = filterbank_buf_write_string(buf, "az_start");
  buf = filterbank_buf_write_double(buf, hdr->az_start);
  buf = filterbank_buf_write_string(buf, "za_start");
  buf = filterbank_buf_write_double(buf, hdr->za_start);
  buf = filterbank_buf_write_string(buf, "data_type");
  buf = filterbank_buf_write_int(   buf, hdr->data_type);
  buf = filterbank_buf_write_string(buf, "fch1");
  buf = filterbank_buf_write_double(buf, hdr->fch1);
  buf = filterbank_buf_write_string(buf, "foff");
  buf = filterbank_buf_write_double(buf, hdr->foff);
  buf = filterbank_buf_write_string(buf, "nchans");
  buf = filterbank_buf_write_int(   buf, hdr->nchans);
  buf = filterbank_buf_write_string(buf, "nbeams");
  buf = filterbank_buf_write_int(   buf, hdr->nbeams);
  buf = filterbank_buf_write_string(buf, "ibeam");
  buf = filterbank_buf_write_int(   buf, hdr->ibeam);
  buf = filterbank_buf_write_string(buf, "nbits");
  buf = filterbank_buf_write_int(   buf, hdr->nbits);
  buf = filterbank_buf_write_string(buf, "tstart");
  buf = filterbank_buf_write_double(buf, hdr->tstart);
  buf = filterbank_buf_write_string(buf, "tsamp");
  buf = filterbank_buf_write_double(buf, hdr->tsamp);
  buf = filterbank_buf_write_string(buf, "nifs");
  buf = filterbank_buf_write_int(   buf, hdr->nifs);
  if(hdr->barycentric) {
    buf = filterbank_buf_write_string(buf, "barycentric");
    buf = filterbank_buf_write_int(   buf, hdr->barycentric);
  }
  if(hdr->pulsarcentric) {
    buf = filterbank_buf_write_string(buf, "pulsarcentric");
    buf = filterbank_buf_write_int(   buf, hdr->pulsarcentric);
  }
  buf = filterbank_buf_write_string(buf, "source_name");
  buf = filterbank_buf_write_string(buf, hdr->source_name);

  // Make strlen(padstr) be 79-(4+11+4)
  padstr[79-19] = '\0';
  padlen = minlen - (buf - buf0)
         - (2*sizeof(int32_t)+strlen("rawdatafile")+strlen(hdr->rawdatafile))
         - (  sizeof(int32_t)+strlen("HEADER_END"));
  while(padlen > 98) {
    buf = filterbank_buf_write_string(buf, "rawdatafile");
    buf = filterbank_buf_write_string(buf, padstr);
    padlen -= 79;
  }
  if(padlen > 19) {
    padstr[79-19] = ' ';
    padstr[padlen-19] = '\0';
    buf = filterbank_buf_write_string(buf, "rawdatafile");
    buf = filterbank_buf_write_string(buf, padstr);
  }

  buf = filterbank_buf_write_string(buf, "rawdatafile");
  buf = filterbank_buf_write_string(buf, hdr->rawdatafile);
  buf = filterbank_buf_write_string(buf, "HEADER_END");

  return buf;
}

void * filterbank_buf_write_header(void * buf, const filterbank_header_t * hdr)
{
  return filterbank_buf_write_padded_header(buf, hdr, 0);
}

// TODO Make this more robust by using the value of *hdr_len on enrty as the
// max number of bytes to process from buf.
void * filterbank_buf_read_header(void * buf, filterbank_header_t * hdr, size_t * hdr_len)
{
  int32_t len;
  char * kw;
  void * p;

  // No NULLs allowed!
  if(!buf || !hdr || !hdr_len) {
    return buf;
  }

  // Zero out the header structure
  memset(hdr, 0, sizeof(filterbank_header_t));

  // Peek at first keyword
  p = filterbank_buf_peek_string(buf, &kw, &len);
  // If first string is not HEADER_START
  if(strncmp(kw, "HEADER_START", len)) {
    // buf is not a filterbank header, return original buf value.
    return buf;
  }

  // Read next keyword
  p = filterbank_buf_peek_string(p, &kw, &len);

  // While we're not at the end
  while(strncmp(kw, "HEADER_END", len)) {

    // Read and store value for keyword
    if(!strncmp(kw, "machine_id", len)) {
      p = filterbank_buf_read_int(p, &hdr->machine_id);
    } else if(!strncmp(kw, "telescope_id", len)) {
      p = filterbank_buf_read_int(p, &hdr->telescope_id);
    } else if(!strncmp(kw, "data_type", len)) {
      p = filterbank_buf_read_int(p, &hdr->data_type);
    } else if(!strncmp(kw, "barycentric", len)) {
      p = filterbank_buf_read_int(p, &hdr->barycentric);
    } else if(!strncmp(kw, "pulsarcentric", len)) {
      p = filterbank_buf_read_int(p, &hdr->pulsarcentric);
    } else if(!strncmp(kw, "src_raj", len)) {
      p = filterbank_buf_read_angle(p, &hdr->src_raj);
    } else if(!strncmp(kw, "src_dej", len)) {
      p = filterbank_buf_read_angle(p, &hdr->src_dej);
    } else if(!strncmp(kw, "az_start", len)) {
      p = filterbank_buf_read_double(p, &hdr->az_start);
    } else if(!strncmp(kw, "za_start", len)) {
      p = filterbank_buf_read_double(p, &hdr->za_start);
    } else if(!strncmp(kw, "fch1", len)) {
      p = filterbank_buf_read_double(p, &hdr->fch1);
    } else if(!strncmp(kw, "foff", len)) {
      p = filterbank_buf_read_double(p, &hdr->foff);
    } else if(!strncmp(kw, "nchans", len)) {
      p = filterbank_buf_read_int(p, &hdr->nchans);
    } else if(!strncmp(kw, "nbeams", len)) {
      p = filterbank_buf_read_int(p, &hdr->nbeams);
    } else if(!strncmp(kw, "ibeam", len)) {
      p = filterbank_buf_read_int(p, &hdr->ibeam);
    } else if(!strncmp(kw, "nbits", len)) {
      p = filterbank_buf_read_int(p, &hdr->nbits);
    } else if(!strncmp(kw, "tstart", len)) {
      p = filterbank_buf_read_double(p, &hdr->tstart);
    } else if(!strncmp(kw, "tsamp", len)) {
      p = filterbank_buf_read_double(p, &hdr->tsamp);
    } else if(!strncmp(kw, "nifs", len)) {
      p = filterbank_buf_read_int(p, &hdr->nifs);
    } else if(!strncmp(kw, "source_name", len)) {
      len = sizeof(hdr->source_name) - 1;
      p = filterbank_buf_read_string(p, hdr->source_name, &len);
      hdr->source_name[len] = '\0';
    } else if(!strncmp(kw, "rawdatafile", len)) {
      len = sizeof(hdr->rawdatafile) - 1;
      p = filterbank_buf_read_string(p, hdr->rawdatafile, &len);
      hdr->rawdatafile[len] = '\0';
    } else {
      // Ignore unknown keyword
    }

    // Peek next keyword
    p = filterbank_buf_peek_string(p, &kw, &len);
  }

  // Store length
  if(hdr_len) {
    *hdr_len = p - buf;
  }

  return p;
}

ssize_t filterbank_fd_read_header(int fd, filterbank_header_t * hdr, size_t * hdr_len)
{
  char buf[4096];
  char * p;
  size_t len;
  size_t bytes_read;

  // Make sure hdr is non-NULL
  if(!hdr) {
    return -1;
  }

  // Read more than the header could ever be
  len = bytes_read = read(fd, buf, sizeof(buf));

  // Parse header from buffer
  p = filterbank_buf_read_header(buf, hdr, &len);

  // If successful
  if(p != buf) {
    // Seek to end of header
    lseek(fd, len - 4096, SEEK_CUR);
    // Store length
    if(hdr_len) {
      *hdr_len = len;
    }
  }

  return len;
}
