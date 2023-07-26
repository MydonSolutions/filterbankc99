#ifndef FILTERBANK_C99_H5_H_
#define FILTERBANK_C99_H5_H_

#include "hdf5/serial/hdf5.h"
#include <string.h>
#include <stdlib.h>

#include "h5dsc99/h5_dataspace.h"
#include "filterbankc99/filterbank_header.h"
#include "filterbankc99/filterbank_printer.h"

typedef struct
{
  filterbank_header_t header;
  size_t ntimes_per_write;
  void *data;
  hid_t file_id;
  uint8_t *mask;
  H5_open_dataspace_t ds_data;
  H5_open_dataspace_t ds_mask;
} filterbank_h5_file_t;

#ifdef __cplusplus
extern "C" {
#endif

int filterbank_h5_open_explicit(char* filepath, filterbank_h5_file_t *fbh5file, hid_t Tdata);

static inline int filterbank_h5_open(char* filepath, filterbank_h5_file_t *fbh5file) {
	hid_t elem_type;
	switch(fbh5file->header.nbits) {
		case 8:
			elem_type = H5T_NATIVE_B8;
			break;
		case 16:
			elem_type = H5T_NATIVE_B16;
			break;
		case 32:
			elem_type = H5T_NATIVE_FLOAT;
			break;
		case 64:
			elem_type = H5T_NATIVE_DOUBLE;
			break;
		default:
			return 1;
	}
	return filterbank_h5_open_explicit(filepath, fbh5file, H5Tcopy(elem_type));
}

int filterbank_h5_change_ntimes_per_write(filterbank_h5_file_t* fbh5file, size_t ntimes_per_write);

void filterbank_h5_alloc(filterbank_h5_file_t *fbh5file);

void filterbank_h5_clear_alloc(filterbank_h5_file_t *fbh5file);

void filterbank_h5_free(filterbank_h5_file_t *fbh5file);

void filterbank_h5_close(filterbank_h5_file_t *fbh5file);

int filterbank_h5_write(filterbank_h5_file_t* fbh5file);
int filterbank_h5_write_FTP(filterbank_h5_file_t* fbh5file);
int filterbank_h5_write_FTP_reversed(filterbank_h5_file_t* fbh5file);

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_C99_H5_H_