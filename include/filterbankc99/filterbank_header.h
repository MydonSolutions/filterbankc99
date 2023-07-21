#ifndef FILTERBANK_C99_HEADER_H_
#define FILTERBANK_C99_HEADER_H_

typedef struct {
  // 0=fake data; 1=Arecibo; 2=Ooty... others to be added
  int machine_id;
  // 0=FAKE; 1=PSPM; 2=WAPP; 3=OOTY... others to be added
  int telescope_id;
  // 1=filterbank; 2=time series... others to be added
  int data_type;
  // 1 if barycentric or 0 otherwise (only output if non-zero)
  int barycentric;
  // 1 if pulsarcentric or 0 otherwise (only output if non-zero)
  int pulsarcentric;
  // right ascension (J2000) of source (hours)
  // will be converted to/from hhmmss.s
  double src_raj;
  // declination (J2000) of source (degrees)
  // will be converted to/from ddmmss.s
  double src_dej;
  // telescope azimuth at start of scan (degrees)
  double az_start;
  // telescope zenith angle at start of scan (degrees)
  double za_start;
  // centre frequency (MHz) of first filterbank channel
  double fch1;
  // filterbank channel bandwidth (MHz)
  double foff;
  // number of fine filterbank channels (not coarse channels)
  int nchans;
  // total number of beams
  int nbeams;
  // total number of beams
  int ibeam;
  // number of bits per time sample
  int nbits;
  // time stamp (MJD) of first sample
  double tstart;
  // time interval between samples (s)
  double tsamp;
  // number of seperate IF channels
  int nifs;
  // the name of the source being observed by the telescope
  char source_name[81];
  // the name of the original data file
  char rawdatafile[81];

  // ------------------------ new for FBH5 ------------------------------------
  // Number of fine channels per coarse channel
  // E.g. turbo_seti will compute the number of coarse channels = nchans / nfpc
  int nfpc;
  
} filterbank_header_t;


#ifdef __cplusplus
extern "C" {
#endif

// Header functions

ssize_t filterbank_fd_write_padded_header(int fd, const filterbank_header_t * hdr, int32_t minlen);
ssize_t filterbank_fd_write_header(int fd, const filterbank_header_t * hdr);
void * filterbank_buf_write_padded_header(void * buf, const filterbank_header_t * hdr, int32_t minlen);
void * filterbank_buf_write_header(void * buf, const filterbank_header_t * hdr);

ssize_t filterbank_fd_read_header(int fd, filterbank_header_t * hdr, size_t * hdr_len);
void * filterbank_buf_read_header(void * buf, filterbank_header_t * hdr, size_t * hdr_len);

#ifdef __cplusplus
}
#endif

#endif // FILTERBANK_HEADER_H_
