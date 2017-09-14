/*  10/09/06 jwb Added two includes to make lseek behave properly
 *               under Mac OS X.
 */
#include <sys/types.h>					    /* jwb 10/09/06 */
#include <unistd.h>					    /* jwb 10/09/06 */
/*--------------------------------------------------*/
/*    HEADER structure to hold                      */
/*      salient data for analysis/synthesis files   */
/*--------------------------------------------------*/
typedef struct{ 
	 char *performer;     /* name of performer                        */
	 char *instrument;    /* instrument used                          */
	 char *date;          /* date of recording                        */
	 char *pitch;         /* pitch played                             */
	 char *dyn;           /* dynamic level                            */
	 char *vibra;         /* vibrato (YES/NO)                         */
	 char *part;          /* portion of tone (beg., middle, end, all) */
	 char *type;          /* "full" or "compressed" data format       */
	 char *comments;      /* additional comments                      */
	 char *andate;        /* date of analysis, if analysis file       */
        float  interpval;     /* analysis reinterp. factor                */
        float  sr;            /* signal sample rate                       */
	float  tl;            /* tone length, seconds                     */
	float  smax;          /* max. amplitude of input signal           */
	float  fa;            /* fundamental freq. assumed in analysis    */
	float  dt;            /* time between analysis blocks, seconds    */
	  int  fftlen;        /* analysis block size                      */
	  int  nhar;          /* number of harmonics                      */
	  int  nchans;        /* number of channels recorded              */
	  int  npts;          /* number of analysis blocks                */
	  } HEADER;  	      /* data type for header structure           */

void rdat(), wdat();   /*  function declarations for calling routines     */

