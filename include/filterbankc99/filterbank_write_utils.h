#ifndef FILTERBANK_C99_WRITE_H_
#define FILTERBANK_C99_WRITE_H_

#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <limits.h>

#ifdef __cplusplus
extern "C" {
#endif

// Write utilities

ssize_t filterbank_fd_write_int(int fd, int32_t i);
void * filterbank_buf_write_int(void * buf, int32_t i);

ssize_t filterbank_fd_write_double(int fd, double d);
void * filterbank_buf_write_double(void * buf, double d);

ssize_t filterbank_fd_write_angle(int fd, double d);
void * filterbank_buf_write_angle(void * buf, double d);

ssize_t filterbank_fd_write_string(int fd, const char * c);
void * filterbank_buf_write_string(void * buf, const char * c);

ssize_t filterbank_write_FTP(
  const int fd,
  void* data,
  const size_t n_bytes,
  const size_t n_channels,
  const size_t n_polarizations,
  const size_t n_samplebits
);

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_C99_WRITE_H_