#ifndef FILTERBANK_C99_WRITE_H_
#define FILTERBANK_C99_WRITE_H_

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <limits.h>

#include "filterbankc99/filterbank_header.h"
#include "filterbankc99/filterbank_printer.h"

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

// Utilities mimicking those for h5 for easy swapout

typedef struct
{
  filterbank_header_t header;
  void *data;
  int file_descriptor;
} filterbank_file_t;

int filterbank_open(char* filepath, filterbank_file_t *fbfile);

static inline size_t _filterbank_data_bytesize(filterbank_file_t *fbfile) {
  return fbfile->header.nifs*fbfile->header.nchans*fbfile->header.nbits/8;
}

static inline void filterbank_alloc(filterbank_file_t *fbfile) {
  fbfile->data = malloc(_filterbank_data_bytesize(fbfile));
}

static inline void filterbank_clear_alloc(filterbank_file_t *fbfile) {
  memset(fbfile->data, 0, _filterbank_data_bytesize(fbfile));
}

static inline void filterbank_free(filterbank_file_t *fbfile) {
  free(fbfile->data);
}

static inline void filterbank_close(filterbank_file_t *fbfile) {
  close(fbfile->file_descriptor);
}

static inline int filterbank_write_dynamic(filterbank_file_t* fbfile) {
  return write(
    fbfile->file_descriptor,
    fbfile->data,
    _filterbank_data_bytesize(fbfile)
  );
}

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_C99_WRITE_H_