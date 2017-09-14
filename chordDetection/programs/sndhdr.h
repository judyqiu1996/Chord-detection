/****************************************************************************
 * 
 *   sndhdr.h -- Declarations of structures and functions to support
 *               the reading and writing of '.snd' (Sun/NeXT sound file)
 *               headers.
 *
 *   Based on the declarations which formerly appeared in Chris Gennaula's
 *   'soundstruct.h' (01/28/91), then in 'SlamHdr.c' (jwb 03/10/94).
 *
 *   Changes:                        
 *  02/04/98 jjjm	Moved these declarations from SlamHdr.c to this file
 *                      so all programs in the SNDAN suite which read/write
 *                      '.snd' files use a common set of decl. & functions
 *                      (replacing NXT_HDR, getnexthead, rdNxtHdr, wrNxtHdr)
 *  03/28/98 jjjm	Added a definition for the little-endian equivalent
 *                      of the SND magic to simplify testing on l-e machines.
 *                      Added the function fixSndHdr() to correct the header
 *                      dataSize field after all samples have been written.
 *  04/02/98 jwb	Cosmetics.
 *  10/09/06 jwb	Add includes for lseek on OS X.
 *****************************************************************************/
#include <sys/types.h>					    /* jwb 10/09/06 */
#include <unistd.h>					    /* jwb 10/09/06 */
#define SND_MAGIC ((int)0x2e736e64)			  /* jwb 03/10/94 */
#define SND_MAGIC_LE ((int)0x646e732e)			 /* jjjm 03/28/98 */
#define SND_FORMAT_LINEAR_16		(3)		  /* jwb 03/10/94 */
#define SND_FORMAT_FLOAT_32		(6)		  /* jjjm 02/04/98 */

typedef struct {					  /* jwb 03/10/94 */
    int magic;		/* must be equal to '.snd' */     /* jwb 04/02/98 */
    int dataLocation;	/* Offset or pointer to the raw data */
    int dataSize;	/* Number of bytes of data in the raw data */
    int dataFormat;	/* The data format code */
    int samplingRate;	/* The sampling rate */
    int channelCount;	/* The number of channels */
    char info[4];	/* Textual information relating to the sound. */
} SND_HDR;

void readSndHdr( int fd, int* sr, int* nchans, int* nsamps, int* samptype );

void writeSndHdr( int fd, int sr, int nchans, int nsamps, int samptype );

void fixSndHdr( int fd );
