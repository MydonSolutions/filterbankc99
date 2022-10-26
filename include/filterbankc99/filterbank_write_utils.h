#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <limits.h>

ssize_t filterbank_write_FTP(
  const int fd,
  void* data,
  const size_t n_channels,
  const size_t n_time_samples,
  const size_t n_polarizations,
  const size_t n_bytes
);
