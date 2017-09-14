/*
 * wavhdr.h  -- include file for WAVE file handling utilities set
 *
 * Jonathan Mohr
 * Augustana University College
 * Camrose, Alberta, Canada T4V 2R3
 * mohrj@augustana.ab.ca
 *
 * 1994 Nov 25
 *
 * Revised 1998 Mar 29 to simplify the handling of WAVE files on
 * big-endian architectures.
 * Changes:
 *  10/09/06 jwb	Add includes for lseek on OS X.
 */

#include <sys/types.h>					    /* jwb 10/09/06 */
#include <unistd.h>					    /* jwb 10/09/06 */

#ifndef __WAV__
#define __WAV__

/*
 * ---------------------------------------------------------------------
 *  definitions for RIFF-output (from Windows MMSYSTEM)
 * ---------------------------------------------------------------------
 */

typedef unsigned char  BYTE;	/* must be 1 byte (8 bits) */
typedef unsigned short WORD;	/* must be 2 bytes (16 bits) */
typedef unsigned long  DWORD;	/* must be 4 bytes (32 bits) */

typedef DWORD FOURCC;	/* a four character code */

typedef struct CHUNKHDR {
  FOURCC ckid;		/* chunk ID */
  DWORD  dwSize; 	/* chunk size */
} CHUNKHDR;


/* specific waveform format structure for PCM data */
typedef struct pcmwaveformat_tag {
  WORD  wFormatTag;	  /* format type */
  WORD  wChannels;	  /* number of channels (i.e. mono, stereo, etc.) */
  DWORD dwSamplesPerSec;  /* sample rate */
  DWORD dwAvgBytesPerSec; /* for buffer estimation */
  WORD  wBlockAlign;	  /* block size of data */
  WORD  wBitsPerSample;
} PCMWAVEFMT;


/* MMIO macros */
#define mmioFOURCC(ch0, ch1, ch2, ch3) \
  ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | \
  ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24))

/* big-endian equivalent                                    03/29/98 jjjm */
#define mmioFOURCC_BE(ch0, ch1, ch2, ch3) \
  ((DWORD)(BYTE)(ch3) | ((DWORD)(BYTE)(ch2) << 8) | \
  ((DWORD)(BYTE)(ch1) << 16) | ((DWORD)(BYTE)(ch0) << 24))

#define FOURCC_RIFF	mmioFOURCC ('R', 'I', 'F', 'F')
#define FOURCC_LIST	mmioFOURCC ('L', 'I', 'S', 'T')
#define FOURCC_WAVE	mmioFOURCC ('W', 'A', 'V', 'E')
#define FOURCC_FMT	mmioFOURCC ('f', 'm', 't', ' ')
#define FOURCC_DATA	mmioFOURCC ('d', 'a', 't', 'a')

/* big-endian equivalents                                   03/29/98 jjjm */
#define FOURCC_BE_RIFF	mmioFOURCC_BE ('R', 'I', 'F', 'F')
#define FOURCC_BE_LIST	mmioFOURCC_BE ('L', 'I', 'S', 'T')
#define FOURCC_BE_WAVE	mmioFOURCC_BE ('W', 'A', 'V', 'E')
#define FOURCC_BE_FMT	mmioFOURCC_BE ('f', 'm', 't', ' ')
#define FOURCC_BE_DATA	mmioFOURCC_BE ('d', 'a', 't', 'a')


/* simplified Header for standard WAV files */
typedef struct WAV_HDR {
  CHUNKHDR   chkRiff;
  FOURCC     fccWave;
  CHUNKHDR   chkFmt;
  PCMWAVEFMT pcmFmt;
  CHUNKHDR   chkData;
} WAV_HDR;


#define IS_STD_WAV_HEADER(waveHdr) ( \
  waveHdr.chkRiff.ckid == FOURCC_RIFF && \
  waveHdr.fccWave == FOURCC_WAVE && \
  waveHdr.chkFmt.ckid == FOURCC_FMT && \
  waveHdr.chkData.ckid == FOURCC_DATA && \
  waveHdr.pcmFmt.wFormatTag == WAVE_FORMAT_PCM)

/* big-endian equivalent                                    03/29/98 jjjm */
#define IS_STD_WAV_HEADER_BE(waveHdr) ( \
  waveHdr.chkRiff.ckid == FOURCC_BE_RIFF && \
  waveHdr.fccWave == FOURCC_BE_WAVE && \
  waveHdr.chkFmt.ckid == FOURCC_BE_FMT && \
  waveHdr.chkData.ckid == FOURCC_BE_DATA && \
  waveHdr.pcmFmt.wFormatTag == (0x0100))

/* flags for 'wFormatTag' field of WAVEFORMAT */
#define	WAVE_FORMAT_UNKNOWN		(0x0000)
#define	WAVE_FORMAT_PCM			(0x0001) 
#define	WAVE_FORMAT_ADPCM		(0x0002)
#define	WAVE_FORMAT_ALAW		(0x0006)
#define	WAVE_FORMAT_MULAW		(0x0007)
#define	WAVE_FORMAT_OKI_ADPCM		(0x0010)
#define	WAVE_FORMAT_DIGISTD		(0x0015)
#define	WAVE_FORMAT_DIGIFIX		(0x0016)
#define	IBM_FORMAT_MULAW         	(0x0101)
#define	IBM_FORMAT_ALAW			(0x0102)
#define	IBM_FORMAT_ADPCM         	(0x0103)

/* function prototypes */

/*
 *  readWavHdr() -- Read a Windows 3.0 WAVE file header. 
 *
 *  This provides information such as the sampling rate, 
 *  size of samples, and number of channels (mono/stereo/quad).
 *
 *  Note that a WAVE header is not necessarily of a fixed size.
 *  While a minimal WAVE header is 44 bytes, each chunk header
 *  contains a size field which must be used to locate the start
 *  of the next chunk.
 */

void readWavHdr( int fd, int* sr, int* nchans, int* nsamps, int* sampsize );

/*
 *  writeWavHdr() -- Write a Windows 3.0 WAVE file header. 
 *
 *  Parameters: 'fd' is the file descriptor of the open file, 
 *    'sr' is the sample rate, 'nchans' is the number of channels
 *    (1 for mono, 2 for stereo, or 4 for quad), 'nsamps' is the
 *    number of samples which are expected to be written (or zero),
 *    and 'sampsize' is the number of bits per sample. 
 *
 *  The number of samples in the WAVE file should be checked (and the header
 *  corrected as required) by calling fixWavHdr() after all samples have 
 *  been written.
 *
 *  The SNDAN suite only supports 16-bit samples for WAVE files.
 *  (The WAVE format does not support float samples.)
 */

void writeWavHdr( int fd, int sr, int nchans, int nsamps, int sampsize );


/*
 *  fixWavHdr() -- Change the values in a WAVE file header which depend
 *                 on the size of the data chunk of the WAVE file.
 *
 *  fixWavHdr() should be called at the end of any program that creates a
 *  WAVE file by calling writeWavHdr() and then writing samples to the file.
 *  It adjusts the file and data size values in the WAVE header based on
 *  the actual size of the file.  (It is typical to specify the size of the
 *  data chunk as zero when first creating the WAVE file, but the resulting
 *  header must be updated when all data have been written or the sound
 *  file will not be playable by most utilities, e.g., 'wavplay' or 'sox').
 */

void fixWavHdr( int fd );

#endif
