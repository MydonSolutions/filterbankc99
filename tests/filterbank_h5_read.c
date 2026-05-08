#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filterbankc99.h"

filterbank_h5_file_t filterbank_h5_access_file_explicit(
  char *filepath,
  hid_t Paccess
) {
  filterbank_h5_file_t fbh5 = {0};
  fbh5.file_id = H5Fopen(filepath, H5F_ACC_RDONLY, Paccess);

  if (fbh5.file_id == H5I_INVALID_HID) {
    filterbank_print_error("Failed to open file: '%s'", filepath);
    return fbh5;
  }
  fbh5.ds_mask.name = "mask";
  H5DSaccess(fbh5.file_id, H5P_DEFAULT, &fbh5.ds_mask);
  fbh5.ds_data.name = "data";
  H5DSaccess(fbh5.file_id, H5P_DEFAULT, &fbh5.ds_data);

  filterbank_h5_read_header(fbh5.ds_data.D_id, &fbh5.header);

  return fbh5;
}

int main(int argc, char * argv[])
{
#ifndef HDF5
  fprintf(stderr, "HDF5 is not defined...");
  return 1;
#endif

  filterbank_h5_file_t fbh5 = filterbank_h5_access_file_explicit(
    argc > 1 ? argv[1] : "./fbutils_h5.00.fbh5",
    H5P_DEFAULT
  );
  if (fbh5.file_id == H5I_INVALID_HID) {
    return 1;
  }

  char* attr_class = H5DSread_all(
    fbh5.file_id,
    "CLASS"
  );
  printf("Class: %s\n", attr_class);
  free(attr_class);
  char* attr_version = H5DSread_all(
    fbh5.file_id,
    "VERSION"
  );
  printf("Version: %s\n", attr_version);
  free(attr_version);

  printf("az_start: %f\n", fbh5.header.az_start);
  printf("za_start: %f\n", fbh5.header.za_start);
  printf("barycentric: %d\n", fbh5.header.barycentric);
  printf("data_type: %d\n", fbh5.header.data_type);
  printf("fch1: %f\n", fbh5.header.fch1);
  printf("foff: %f\n", fbh5.header.foff);
  printf("ibeam: %d\n", fbh5.header.ibeam);
  printf("nbeams: %d\n", fbh5.header.nbeams);
  printf("machine_id: %d\n", fbh5.header.machine_id);
  printf("nbits: %d\n", fbh5.header.nbits);
  printf("nchans: %d\n", fbh5.header.nchans);
  printf("nifs: %d\n", fbh5.header.nifs);
  printf("pulsarcentric: %d\n", fbh5.header.pulsarcentric);
  printf("rawdatafile: '%s'\n", fbh5.header.rawdatafile);
  printf("source_name: '%s'\n", fbh5.header.source_name);
  printf("src_dej: %f\n", fbh5.header.src_dej);
  printf("src_raj: %f\n", fbh5.header.src_raj);
  printf("tsamp: %f\n", fbh5.header.tsamp);
  printf("tstart: %f\n", fbh5.header.tstart);
  printf("telescope_id: %d\n", fbh5.header.telescope_id);
  printf("nfpc: %d\n", fbh5.header.nfpc);

  printf("Data elements: %ld (rank=%d)\n", H5DSnelem(&fbh5.ds_data), fbh5.ds_data.rank);
  H5_open_dataspace_t h5ds_dim_labels = {0};
  h5ds_dim_labels.name = "DIMENSION_LABELS";
  H5DSaccess(fbh5.ds_data.D_id, H5P_DEFAULT, &h5ds_dim_labels);
  char **attr_dim_labels = H5DSmalloc(&h5ds_dim_labels);
  H5DSread(&h5ds_dim_labels, attr_dim_labels);
  for (int i = 0; i < h5ds_dim_labels.dims[0]; i++) {
    printf("\t#%d: %ld (%s)\n", i, fbh5.ds_data.dims[i], attr_dim_labels[i]);
    free(attr_dim_labels[i]);
  }
  free(attr_dim_labels);

  return 0;
}