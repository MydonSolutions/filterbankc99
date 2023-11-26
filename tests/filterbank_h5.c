#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filterbankc99.h"


int main(int argc, char * argv[])
{
#ifndef HDF5
  return 1;
#else
  
  filterbank_header_t hdr = {0};

  // 0=fake data; 1=Arecibo; 2=Ooty... others to be added
  hdr.machine_id = 20; // ???
  // 0=FAKE; 1=PSPM; 2=WAPP; 3=OOTY... others to be added
  hdr.telescope_id = 6; // GBT
  // 1=filterbank; 2=time series... others to be added
  hdr.data_type = 1;
  // 1 if barycentric or 0 otherwise (only output if non-zero)
  hdr.barycentric = 1;
  // 1 if pulsarcentric or 0 otherwise (only output if non-zero)
  hdr.pulsarcentric = 1;
  // right ascension (J2000) of source (hours)
  // will be converted to/from hhmmss.s
  hdr.src_raj = 20.0 + 39/60.0 + 7.4/3600.0;
  // declination (J2000) of source (degrees)
  // will be converted to/from ddmmss.s
  hdr.src_dej = 42.0 + 24/60.0 + 24.5/3600.0;
  // telescope azimuth at start of scan (degrees)
  hdr.az_start = 12.3456;
  // telescope zenith angle at start of scan (degrees)
  hdr.za_start = 65.4321;
  // centre frequency (MHz) of first filterbank channel
  hdr.fch1 = 4626.464842353016138;
  // filterbank channel bandwidth (MHz)
  hdr.foff = -0.000002793967724;
  // number of filterbank channels
  hdr.nchans = 128;
  // total number of beams
  hdr.nbeams = 1;
  // total number of beams
  hdr.ibeam = 1;
  // number of bits per time sample
  hdr.nbits = sizeof(float)*8;
  // time stamp (MJD) of first sample
  hdr.tstart = 57856.810798611114;
  // time interval between samples (s)
  hdr.tsamp = 1.825361100800;
  // number of seperate IF channels
  hdr.nifs = 1;
  // the name of the source being observed by the telescope
  // Max string size is supposed to be 80, but bug in sigproc if over 79
  strcpy(hdr.source_name, "1234567890123456789012345678901234567890123456789012345678901234567890123456789");
  // the name of the original data file
  // Max string size is supposed to be 80, but bug in sigproc if over 79
  // If not allocated a string longer than 0 characters, it is not written out
  // strcpy(hdr.rawdatafile, "1234567890123456789012345678901234567890123456789012345678901234567890123456789");

  if(argc > 1) {
    int fd  = open(argv[1],  O_RDONLY);
    ssize_t hdr_size = filterbank_fd_read_header(fd, &hdr, NULL);
    printf("header size %lu bytes\n", hdr_size);
    printf("fch1 %.17g\n", hdr.fch1);
    printf("foff %.17g\n", hdr.foff);

    return 0;
  }
  int i=0, t=0, c=0;
  size_t single_time_bytesize = filterbank_data_bytesize(&hdr);
  float* floats_ftp_ordering = NULL;
  float* floats_tpf_ordering = NULL;

  char fname[80];
  i = 0;

  // HDF5 via struct
  sprintf(fname, "fbutils_h5.%02d.fbh5", i);

  filterbank_h5_file_t fbh5_file = {0};
  memcpy(&fbh5_file.header, &hdr, sizeof(filterbank_header_t));
  filterbank_h5_open(fname, &fbh5_file);
  // allocate the data and mask pointers, then clear them
  filterbank_h5_alloc(&fbh5_file);

  const int ntimes_per_write = 3;
  // oh, update the fbh5 data to match that of the fil file
  filterbank_h5_change_ntimes_per_write(&fbh5_file, ntimes_per_write);
  filterbank_h5_free(&fbh5_file); // free old pointers
  filterbank_h5_alloc(&fbh5_file); // make new pointers
  filterbank_h5_clear_alloc(&fbh5_file); // memset(0) new pointers
  free(fbh5_file.data); // free the allocated data pointer
  fbh5_file.data = floats_tpf_ordering; // manually allocate the data pointer

  // FTP and TPF data malloc
  const size_t floats_size = ntimes_per_write
    * hdr.nifs
    * hdr.nchans
    * sizeof(float);
  floats_tpf_ordering = malloc(floats_size);
  floats_ftp_ordering = malloc(floats_size);


  for(t=0; t < ntimes_per_write; t++) {
    for (i = 0; i < hdr.nifs; i ++) {
      for (c = 0; c < hdr.nchans; c ++) {
        float sample = t*1000.0 + c + 1.0;

        floats_tpf_ordering[(t*hdr.nifs + i)*hdr.nchans + c] = sample;
        floats_ftp_ordering[(c*ntimes_per_write + t)*hdr.nifs + i] = sample;
      }
    }
  }

  // HDF5 via struct
  filterbank_h5_write(&fbh5_file);

  fbh5_file.data = floats_ftp_ordering;
  filterbank_h5_write_FTP(&fbh5_file);
  filterbank_h5_write_FTP_reversed(&fbh5_file);

  fbh5_file.data = floats_tpf_ordering;
  filterbank_h5_write(&fbh5_file);

  // HDF5 via struct
  filterbank_h5_close(&fbh5_file);
  fbh5_file.data = NULL; // hide the manually controlled data pointer
  filterbank_h5_free(&fbh5_file);

  free(floats_tpf_ordering);
  free(floats_ftp_ordering);
  return 0;
#endif
}