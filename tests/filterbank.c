
#include <stdio.h>
// #include <unistd.h>
#include <stdlib.h>
#include <string.h>
// #include <stdint.h>
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>

#include "filterbankc99.h"
#include "filterbankc99/filterbank_h5.h"

int main(int argc, char * argv[])
{
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
  hdr.nbits = 32;
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
  } else {
    int i = 0;
    size_t floats_byte_size = hdr.nchans*hdr.nifs*sizeof(float);
    float* floats = malloc(floats_byte_size);
    for (int c = 0; c < hdr.nchans; c ++) {
      for (i = 0; i < hdr.nifs; i ++) {
        floats[i*hdr.nchans + c] = 0.0 + c;
      }
    }
    
    char fname[80];
    i = 0;

    // direct file descriptor SIGPROC
    sprintf(fname, "fbutils_fd.%02d.fil", i);
    int fdfd  = open(fname,  O_WRONLY | O_CREAT | O_TRUNC, 0664);
    ssize_t nbytes = filterbank_fd_write_padded_header(fdfd, &hdr, 1024+i);
    printf("%02d: write %lu fd header bytes, ", i, nbytes);

    // character buffer proxy SIGPROC
    sprintf(fname, "fbutils_buf.%02d.fil", i);
    int fdbuf = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    char buf[1024];
    char * end = filterbank_buf_write_padded_header(buf, &hdr, 1024+i);
    nbytes = end-buf;
    write(fdbuf, buf, nbytes);
    printf("write %lu buf header bytes\n", nbytes);

    // SIGPROC via struct
    sprintf(fname, "fbutils_struct.%02d.fil", i);
    filterbank_file_t fb_file = {0};
    memcpy(&fb_file.header, &hdr, sizeof(filterbank_header_t));
    filterbank_open(fname, &fb_file);
    fb_file.data = floats; // manually allocate the data pointer

    // HDF5 via struct
    sprintf(fname, "fbutils_h5.%02d.fbh5", i);

    filterbank_h5_file_t fbh5_file = {0};
    memcpy(&fbh5_file.header, &hdr, sizeof(filterbank_header_t));
    filterbank_h5_open(fname, &fbh5_file);
    // allocate the data and mask pointers, then clear them
    filterbank_h5_alloc(&fbh5_file);
    filterbank_h5_clear_alloc(&fbh5_file);
    free(fbh5_file.data); // free the allocated data pointer
    fbh5_file.data = floats; // manually allocate the data pointer

    for(int t=0; t<3; t++) {
      // direct file descriptor SIGPROC
      write(fdfd, floats, floats_byte_size);
      // character buffer proxy SIGPROC
      write(fdbuf, floats, floats_byte_size);
      // SIGPROC via struct
      filterbank_write_dynamic(&fb_file);
      // HDF5 via struct
      filterbank_h5_write_dynamic(&fbh5_file);
    }

    // direct file descriptor SIGPROC
    close(fdfd);
    // character buffer proxy SIGPROC
    close(fdbuf);
    // SIGPROC via struct
    filterbank_close(&fb_file);
    // HDF5 via struct
    filterbank_h5_close(&fbh5_file);
    fbh5_file.data = NULL; // hide the manual controlled data pointer
    filterbank_h5_free(&fbh5_file);

    free(floats);
  }

  return 0;
}