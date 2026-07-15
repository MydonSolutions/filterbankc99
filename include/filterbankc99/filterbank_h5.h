#ifndef FILTERBANK_C99_H5_H_
#define FILTERBANK_C99_H5_H_

#include <string.h>
#include <stdlib.h>
#include "hdf5.h"

#include "h5dsc99/h5_dataspace.h"
#include "filterbankc99/filterbank_header.h"
#include "filterbankc99/filterbank_printer.h"

typedef struct
{
  filterbank_header_t header;
  size_t ntimes_per_write;
  size_t nchans_per_write;
  void *data;
  hid_t file_id;
  uint8_t *mask;
  H5_open_dataspace_t ds_data;
  H5_open_dataspace_t ds_mask;
} filterbank_h5_file_t;

#ifdef __cplusplus
extern "C" {
#endif

int filterbank_h5_open_explicit(const char* filepath, filterbank_h5_file_t *fbh5file, hid_t Tdata, hid_t Paccess);

static inline int filterbank_h5_open(const char* filepath, filterbank_h5_file_t *fbh5file) {
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
	return filterbank_h5_open_explicit(filepath, fbh5file, H5Tcopy(elem_type), H5P_DEFAULT);
}

int filterbank_h5_change_ntimes_per_write(filterbank_h5_file_t* fbh5file, size_t ntimes_per_write);

void filterbank_h5_alloc(filterbank_h5_file_t *fbh5file);

void filterbank_h5_clear_alloc(filterbank_h5_file_t *fbh5file);

void filterbank_h5_free(filterbank_h5_file_t *fbh5file);

void filterbank_h5_close(filterbank_h5_file_t *fbh5file);

int filterbank_h5_write(filterbank_h5_file_t* fbh5file);
int filterbank_h5_write_FTP(filterbank_h5_file_t* fbh5file);
int filterbank_h5_write_FTP_reversed(filterbank_h5_file_t* fbh5file);


filterbank_h5_file_t filterbank_h5_access_file_explicit(
  const char *filepath,
  hid_t Paccess
);

void filterbank_h5_change_access_chunking(
  filterbank_h5_file_t* fbh5,
  size_t ntimes_per_read,
  size_t nifs_per_read,
  size_t nchans_per_read
);

void filterbank_h5_read_header(
  hid_t data_id,
  filterbank_header_t* fb_header
);

// Returns:
//   -2 if either mask or data hyperslab increment failed
//   -1 if either mask or data read failed
//   1 if the data hyperslab has wrapped back to all zero indices
//   otherwise 0 (indicating more to read)
int filterbank_h5_read(
  filterbank_h5_file_t* fbh5
);

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_C99_H5_H_