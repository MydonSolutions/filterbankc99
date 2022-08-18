
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "filterbankc99.h"

int main(int argc, char * argv[])
{
  filterbank_header_t hdr;

  // 0=fake data; 1=Arecibo; 2=Ooty... others to be added
  hdr.machine_id = 20; // wtf?
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
  hdr.nchans = 1;
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
  strcpy(hdr.rawdatafile, "1234567890123456789012345678901234567890123456789012345678901234567890123456789");

  if(argc > 1) {
    int fd  = open(argv[1],  O_RDONLY);
    ssize_t hdr_size = filterbank_fd_read_header(fd, &hdr, NULL);
    printf("header size %lu bytes\n", hdr_size);
    printf("fch1 %.17g\n", hdr.fch1);
    printf("foff %.17g\n", hdr.foff);
  } else {
    int i;
    float f = 0;
    char fname[80];

    for(i=0; i<100; i++) {
      sprintf(fname, "fbutils_fd.%02d.fil", i);
      int fdfd  = open(fname,  O_WRONLY | O_CREAT | O_TRUNC, 0664);
      sprintf(fname, "fbutils_buf.%02d.fil", i);
      int fdbuf = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0664);


      ssize_t nbytes = filterbank_fd_write_padded_header(fdfd, &hdr, 1024+i);
      write(fdfd, (void *)&f, sizeof(float));
      printf("%02d: write %lu+4 fd bytes, ", i, nbytes);

      char buf[1024];
      char * end = filterbank_buf_write_padded_header(buf, &hdr, 1024+i);
      nbytes = end-buf;
      write(fdbuf, buf, nbytes);
      write(fdbuf, (void *)&f, sizeof(float));
      printf("write %lu+4 buf bytes\n", nbytes);

      close(fdfd);
      close(fdbuf);
    }
  }

  return 0;
}