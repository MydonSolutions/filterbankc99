#include "filterbankc99/filterbank_h5.h"

herr_t _H5AwriteScalar(
  hid_t dest_id,
  char* name,
  hid_t H5T_id,
  void* value
) {
  hid_t S_id = H5Screate(H5S_SCALAR);
  hid_t A_id = H5Acreate2(
    dest_id,
    name,
    H5T_id,
    S_id,
    H5P_DEFAULT,
    H5P_DEFAULT
  );
  herr_t status = H5Awrite(A_id, H5T_id, value);
	if (status > 0 ) {
		filterbank_print_warn(__FUNCTION__, "H5Awrite failed on '%s'.", name);
		return status;
	}
  status += H5Aclose(A_id);
	if (status > 0 ) {
		filterbank_print_warn(__FUNCTION__, "H5Aclose failed on '%s'.", name);
		return status;
	}
  status += H5Sclose(S_id);
	if (status > 0 ) {
		filterbank_print_warn(__FUNCTION__, "H5Sclose failed on '%s'.", name);
		return status;
	}
  return status;
}

int _filterbank_h5_write_attributes(filterbank_h5_file_t *fbh5file) {
  hid_t Tstr_id = H5Tcopy(H5T_C_S1);
  H5Tset_size(Tstr_id, 10);
  if (_H5AwriteScalar(
    fbh5file->file_id,
    "CLASS",
    Tstr_id,
    "FILTERBANK"
  )) {
		return -1;
	}

  Tstr_id = H5Tcopy(H5T_C_S1);
  H5Tset_size(Tstr_id, 3);
  if (_H5AwriteScalar(
    fbh5file->file_id,
    "VERSION",
    Tstr_id,
    "1.0"
  )) {
		return -1;
	}

  const char* data_dimension_labels[] = {
    "time",
    "feed_id",
    "frequency"
  };

  const hsize_t array_3[] = {3};
  hid_t S_id = H5Screate(H5S_SIMPLE);
  H5Sset_extent_simple(S_id, 1, array_3, NULL);
  hid_t T_id = H5Tcopy(H5T_C_S1);
  H5Tset_size(T_id, H5S_UNLIMITED);
  hid_t mask_dimlabels_id = H5Acreate2(
    fbh5file->ds_mask.D_id,
    "DIMENSION_LABELS",
    T_id,
    S_id,
    H5P_DEFAULT,
    H5P_DEFAULT
  );
  T_id = H5Tcopy(H5T_C_S1);
  H5Tset_size(T_id, H5S_UNLIMITED);
  H5Awrite(mask_dimlabels_id, T_id, data_dimension_labels);
  H5Aclose(mask_dimlabels_id);

  hid_t data_dimlabels_id = H5Acreate2(
    fbh5file->ds_data.D_id,
    "DIMENSION_LABELS",
    T_id,
    S_id,
    H5P_DEFAULT,
    H5P_DEFAULT
  );
  H5Awrite(data_dimlabels_id, T_id, data_dimension_labels);

  H5Aclose(data_dimlabels_id);
  H5Sclose(S_id);
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "az_start",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.az_start
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "barycentric",
    H5T_NATIVE_INT,
    &fbh5file->header.barycentric
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "data_type",
    H5T_NATIVE_INT,
    &fbh5file->header.data_type
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "fch1",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.fch1
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "foff",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.foff
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "ibeam",
    H5T_NATIVE_INT,
    &fbh5file->header.ibeam
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "machine_id",
    H5T_NATIVE_INT,
    &fbh5file->header.machine_id
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "nbeams",
    H5T_NATIVE_INT,
    &fbh5file->header.nbeams
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "nbits",
    H5T_NATIVE_INT,
    &fbh5file->header.nbits
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "nchans",
    H5T_NATIVE_INT,
    &fbh5file->header.nchans
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "nifs",
    H5T_NATIVE_INT,
    &fbh5file->header.nifs
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "pulsarcentric",
    H5T_NATIVE_INT,
    &fbh5file->header.pulsarcentric
  )) {
		return -1;
	}
  
	int str_len = strlen(fbh5file->header.rawdatafile);
  if (fbh5file->header.rawdatafile != NULL && str_len > 0) {
    Tstr_id = H5Tcopy(H5T_C_S1);
    H5Tset_size(Tstr_id, str_len);
    if (_H5AwriteScalar(
      fbh5file->ds_data.D_id,
      "rawdatafile",
      Tstr_id,
      fbh5file->header.rawdatafile
    )) {
			return -1;
		}
  }
  
	str_len = strlen(fbh5file->header.source_name);
  if (fbh5file->header.source_name != NULL && str_len > 0) {
    Tstr_id = H5Tcopy(H5T_C_S1);
    H5Tset_size(Tstr_id, str_len);
    if (_H5AwriteScalar(
      fbh5file->ds_data.D_id,
      "source_name",
      Tstr_id,
      fbh5file->header.source_name
    )) {
			return -1;
		}
  }
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "src_dej",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.src_dej
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "src_raj",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.src_raj
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "telescope_id",
    H5T_NATIVE_INT,
    &fbh5file->header.telescope_id
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "tsamp",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.tsamp
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "tstart",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.tstart
  )) {
		return -1;
	}
  
  if (_H5AwriteScalar(
    fbh5file->ds_data.D_id,
    "za_start",
    H5T_NATIVE_DOUBLE,
    &fbh5file->header.za_start
  )) {
		return -1;
	}
  
  if (fbh5file->header.nfpc > 0) {
    if (_H5AwriteScalar(
      fbh5file->ds_data.D_id,
      "nfpc",
      H5T_NATIVE_INT,
      &fbh5file->header.nfpc
    )) {
      return -1;
    }
  }
	return 0;
}

int filterbank_h5_open_explicit(const char* filepath, filterbank_h5_file_t *fbh5file, hid_t Tdata, hid_t Paccess) {
  fbh5file->file_id = H5Fcreate(filepath, H5F_ACC_TRUNC, H5P_DEFAULT, Paccess);
	if (fbh5file->file_id <= 0) {
		filterbank_print_error(__FUNCTION__, "H5Fcreate failed with path '%s'.", filepath);
		return -1;
	}

  if (fbh5file->ntimes_per_write == 0) {
    fbh5file->ntimes_per_write = 1;
  }
  if (fbh5file->nchans_per_write == 0) {
    // auto-scale nchans_per_write for optimal chunks of <= 1 MB
    fbh5file->nchans_per_write = 1024*1024/(fbh5file->ntimes_per_write*fbh5file->header.nifs*H5Tget_size(Tdata));
    if (fbh5file->nchans_per_write > fbh5file->header.nchans) {
      fbh5file->nchans_per_write = fbh5file->header.nchans;
    }
    if (fbh5file->nchans_per_write == 0) {
      fbh5file->nchans_per_write = 1;
    }
    while (fbh5file->header.nchans%fbh5file->nchans_per_write != 0) {
      fbh5file->nchans_per_write -= 1;
    }
    filterbank_print_info(__FUNCTION__, "Set nchans_per_write=%lu attempting to achieve 1 MB chunks.", fbh5file->nchans_per_write);
  }

  const hsize_t dim3_data_lim[] = {H5S_UNLIMITED, fbh5file->header.nifs, fbh5file->header.nchans};
  const hsize_t dim3_data_chunk[] = {fbh5file->ntimes_per_write, fbh5file->header.nifs, fbh5file->nchans_per_write};

  fbh5file->ds_data.name = "data";
  H5DSset(3, dim3_data_lim, dim3_data_chunk, &fbh5file->ds_data);
  H5DSopen(fbh5file->file_id, Tdata, H5Tcopy(Tdata), &fbh5file->ds_data);
	if (fbh5file->ds_data.D_id <= 0) {
		filterbank_print_error(__FUNCTION__, "H5DSopen failed for 'data'.");
		return -1;
	}
  fbh5file->header.nbits = H5Tget_size(fbh5file->ds_data.Tmem_id)*8;

  fbh5file->ds_mask.name = "mask";
  H5DSset(3, dim3_data_lim, dim3_data_chunk, &fbh5file->ds_mask);
  H5DSopen(fbh5file->file_id, H5Tcopy(H5T_STD_U8LE), H5Tcopy(H5T_STD_U8LE), &fbh5file->ds_mask);
	if (fbh5file->ds_mask.D_id <= 0) {
		filterbank_print_error(__FUNCTION__, "H5DSopen failed for 'mask'.");
		return -1;
	}

  if (_filterbank_h5_write_attributes(fbh5file)) {
		filterbank_print_error(__FUNCTION__, "failed to write attributes.");
		return -1;
	}
	return 0;
}

void filterbank_h5_alloc(filterbank_h5_file_t *fbh5file) {
  fbh5file->data = H5DSmalloc(&fbh5file->ds_data);
  fbh5file->mask = (uint8_t*) H5DSmalloc(&fbh5file->ds_mask);
}

void filterbank_h5_clear_alloc(filterbank_h5_file_t *fbh5file) {
  memset(fbh5file->data, 0, H5DSsize(&fbh5file->ds_data));
  memset(fbh5file->mask, 0, H5DSsize(&fbh5file->ds_mask));
}

void filterbank_h5_free(filterbank_h5_file_t *fbh5file) {
  if (fbh5file->data != NULL) {
    free(fbh5file->data);
  }

  if (fbh5file->mask != NULL) {
    free(fbh5file->mask);
  }
}

void filterbank_h5_close(filterbank_h5_file_t *fbh5file) {
  H5DSclose(&fbh5file->ds_data);
  H5DSclose(&fbh5file->ds_mask);
  H5Fclose(fbh5file->file_id);
}

int filterbank_h5_change_ntimes_per_write(filterbank_h5_file_t* fbh5file, size_t ntimes_per_write) {
  herr_t status = 0;
  fbh5file->ds_data.dimchunks[0] = ntimes_per_write;
  fbh5file->ds_data.dims[0] = ntimes_per_write;
  status += H5DSchunk_update(&fbh5file->ds_data);

  fbh5file->ds_mask.dimchunks[0] = ntimes_per_write;
  fbh5file->ds_mask.dims[0] = ntimes_per_write;
  status += H5DSchunk_update(&fbh5file->ds_mask);

  fbh5file->ntimes_per_write = ntimes_per_write;
  return status;
}

int filterbank_h5_write(filterbank_h5_file_t* fbh5file) {
  herr_t status;
  // data
  status = H5DSextend(&fbh5file->ds_data);
  if (status < 0) { filterbank_print_error(__FUNCTION__, "H5DSextend failure on 'data'"); return -1;}
  status = H5DSwrite(&fbh5file->ds_data, fbh5file->data);
  if (status < 0) { filterbank_print_error(__FUNCTION__, "H5DSwrite failure on 'data'"); return -1;}	

  // mask
  status = H5DSextend(&fbh5file->ds_mask);
  if (status < 0) { filterbank_print_error(__FUNCTION__, "H5DSextend failure on 'mask'"); return -1;}
  status = H5DSwrite(&fbh5file->ds_mask, fbh5file->mask);
  if (status < 0) { filterbank_print_error(__FUNCTION__, "H5DSwrite failure on 'mask'"); return -1;}	

  return 0;
}

#define __h5_write_FTP_preamble \
  /* cache the current dims, for the hyperslab start*/\
  hsize_t count[3] = {1, 1, 1};\
  hsize_t block[3] = {fbh5file->ds_data.dimchunks[0], fbh5file->ds_data.dimchunks[1], 1};\
  herr_t status;\
  H5_open_dataspace_t* dataspaces[] = {\
    &fbh5file->ds_data,\
    &fbh5file->ds_mask,\
  };\
  void* dataspace_datapointers[] = {\
    fbh5file->data,\
    fbh5file->mask,\
  };\
  const int dataspace_count = sizeof(dataspaces)/sizeof(H5_open_dataspace_t*);\
  H5_open_dataspace_t* dataspace;\
  for (int dataspace_i = 0; dataspace_i < dataspace_count; dataspace_i++) {\
    dataspace = dataspaces[dataspace_i];\
    status = H5DSextend(dataspace);\
    if (status < 0) { filterbank_print_error(__FUNCTION__, "H5DSextend failure on '%s'", dataspace->name); return -1;}  \
    /*set chunk dimensions to TP1*/\
    dataspace->dimchunks[2] = 1;\
    status = H5DSchunk_update(dataspace);\

#define __h5_write_FTP_innermost \
  status += H5Sclose(dataspace->S_id);\
  dataspace->S_id = H5Dget_space(dataspace->D_id);\
  H5Sselect_hyperslab(\
    dataspace->S_id,\
    H5S_SELECT_SET,\
    dataspace->hyperslab_start,\
    NULL,\
    count,\
    block\
  );\
  status = H5DSwrite(dataspace, ((char*)dataspace_datapointers[dataspace_i]) + f*H5DSsize(dataspace)); \
  if (status < 0) { filterbank_print_error(__FUNCTION__, "H5DSwrite failure on '%s'", dataspace->name); return -1;}

#define __h5_write_FTP_conclusion \
    /*return chunk dimensions to TPF*/\
    dataspace->dimchunks[2] = fbh5file->nchans_per_write;\
    status = H5DSchunk_update(dataspace);\
  }\
  return 0;


int filterbank_h5_write_FTP(filterbank_h5_file_t* fbh5file) {
  __h5_write_FTP_preamble
  for (int f = 0; f < fbh5file->nchans_per_write; f++) {
    __h5_write_FTP_innermost
  }
  __h5_write_FTP_conclusion
}

int filterbank_h5_write_FTP_reversed(filterbank_h5_file_t* fbh5file) {
  __h5_write_FTP_preamble
  for (int f = fbh5file->nchans_per_write; f-- > 0; ) {
    __h5_write_FTP_innermost
  }
  __h5_write_FTP_conclusion
}
