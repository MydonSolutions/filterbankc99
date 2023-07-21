#ifndef FILTERBANK_C99_READ_H_
#define FILTERBANK_C99_READ_H_

#include "filterbankc99/filterbank_utils.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_C99_READ_H_