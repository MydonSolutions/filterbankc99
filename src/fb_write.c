#include "filterbankc99/filterbank_write_utils.h"


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

ssize_t filterbank_write_FTP(
  const int fd,
  void* data,
  const size_t n_bytes,
  const size_t n_channels,
  const size_t n_polarizations,
  const size_t n_samplebits
) {
  const size_t n_time_samples = n_bytes * 8 / (n_channels * n_polarizations * n_samplebits);
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
      for(c = n_channels; c-- > 0; ) {
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

// Utilities mimicking those for h5 for easy swapout

int filterbank_open(char* filepath, filterbank_file_t *fbfile) {
  fbfile->file_descriptor = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0664);
  if (fbfile->file_descriptor <= 0) {
    filterbank_print_error(__FUNCTION__, "open failed with path '%s'.", filepath);
    return -1;
  }
  filterbank_fd_write_header(fbfile->file_descriptor, &fbfile->header);
  return 0;
}
