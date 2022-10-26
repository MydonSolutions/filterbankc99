#include "filterbankc99/filterbank_write_utils.h"

ssize_t filterbank_write_FTP(
  const int fd,
  void* data,
  const size_t n_channels,
  const size_t n_time_samples,
  const size_t n_polarizations,
  const size_t n_bytes
) {
  const size_t chan_bytestride = n_bytes / n_channels;
  const size_t time_bytestride = chan_bytestride / n_time_samples;
  const size_t pol_bytestride = time_bytestride / n_polarizations;

  const long max_iovecs = sysconf(_SC_IOV_MAX);
  struct iovec *iovecs = malloc(max_iovecs * sizeof(struct iovec));
  long n_iovecs = 0;

  ssize_t bytes_written = 0;
  size_t t, p, c;

  for(t = 0; t < n_time_samples; t++) {
    for(p = 0; p < n_polarizations; p++) {
      for(c = 0; c < n_channels; c++) {
        iovecs[n_iovecs].iov_base = data
          + c*chan_bytestride
          + t*time_bytestride
          + p*pol_bytestride;
        iovecs[n_iovecs].iov_len = pol_bytestride;

        if(++n_iovecs == max_iovecs) {
          const ssize_t _bytes_written = writev(fd, iovecs, n_iovecs);
          if(_bytes_written <= 0) { 
            fprintf(stderr, "writev() error: %ld (@ %lu, %lu, %lu)\n", _bytes_written, c, t, p);
            return _bytes_written;
          }
          bytes_written += _bytes_written;
          
          n_iovecs = 0;
        }
      }
    }
  }
  
  if(n_iovecs > 0) {
    const ssize_t _bytes_written = writev(fd, iovecs, n_iovecs);
    if(_bytes_written <= 0) { 
      fprintf(stderr, "writev() error: %ld (@ %lu, %lu, %lu)\n", _bytes_written, c, t, p);
      return _bytes_written;
    }
    bytes_written += _bytes_written;
  }
  
  free(iovecs);
  return bytes_written;
}