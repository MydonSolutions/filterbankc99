#ifndef FILTERBANK_C99_H5_H_
#define FILTERBANK_C99_H5_H_

#include "hdf5/serial/hdf5.h"
#include <string.h>
#include <stdlib.h>

#include "h5dsc99/h5_dataspace.h"
#include "filterbankc99/filterbank_header.h"

typedef struct
{
  hid_t file_id;
  filterbank_header_t header;
  H5_open_dataspace_t ds_data;
  void *data;
  H5_open_dataspace_t ds_mask;
  uint8_t *mask;
} filterbank_h5_file_t;

#ifdef __cplusplus
extern "C" {
#endif

void filterbank_h5_open(char* filepath, filterbank_h5_file_t *fbh5file, hid_t Tdata);

static inline void filterbank_h5_open_float(char* filepath, filterbank_h5_file_t *fbh5file) {
  return filterbank_h5_open(filepath, fbh5file, H5Tcopy(H5T_NATIVE_FLOAT));
}

static inline void filterbank_h5_open_double(char* filepath, filterbank_h5_file_t *fbh5file) {
  return filterbank_h5_open(filepath, fbh5file, H5Tcopy(H5T_NATIVE_DOUBLE));
}

void filterbank_h5_alloc(filterbank_h5_file_t *fbh5file);

void filterbank_h5_clear_alloc(filterbank_h5_file_t *fbh5file);

void filterbank_h5_free(filterbank_h5_file_t *fbh5file);

void filterbank_h5_close(filterbank_h5_file_t *fbh5file);

int filterbank_h5_write_dynamic(filterbank_h5_file_t* fbh5file);

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_C99_H5_H_