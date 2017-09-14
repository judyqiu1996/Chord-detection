/****************************************************************************
				readSamples.c

	Author: James W. Beauchamp
	Date: 02/21/17
	Latest edit: 02/24/17

 ***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define P printf
#define DIAG 0

int openSoundFile(int*, int*);				    /* jwb 02/21/17 */
void readsamps(float*, int, float, int);		    /* jwb 02/21/17 */
extern char* filename;					    /* jwb 02/21/17 */
int byte_reverse;					    /* jwb 02/22/17 */

void readSamples(float** signal,float* sr,int* nsamps)	    /*jwb 02/21/17*/
{
  int i, fd, samplerate, sampN;			    	    /* jwb 02/13/17 */ 
  P("In function readSamples:\n");			    /* jwb 02/13/17 */
  P("Read file %s\n", filename);			    /* jwb 02/23/17 */
  fd = openSoundFile(&samplerate, &sampN);  	    	    /* jwb 02/16/17 */
  *sr = samplerate;					    /* jwb 10/17/16 */
// byte_reverse is a global variable
  if(DIAG)						    /* jwb 02/22/17 */
  {							    /* jwb 02/22/17 */
    P("we are back to readSamples1\n");			    /* jwb 02/21/17 */
    P("fd = %d, sampN = %d, byte_reverse = %d\n",	    /* jwb 02/22/17 */
        fd, sampN, byte_reverse);		    	    /* jwb 02/22/17 */
    P("samplerate = %d, sr = %.1f\n", samplerate,*sr);      /* jwb 10/17/16 */
  }							    /* jwb 02/22/17 */
//  signal array is allocated here and read from file 
//  in readSamples()
  *signal = (float *)calloc(sampN, sizeof(float));     	    /* jwb 02/20/17 */
  readsamps(*signal, fd, *sr, sampN);			    /* jwb 02/21/17 */
  if(DIAG)						    /* jwb 02/22/17 */
  {							    /* jwb 02/22/17 */
    P("we are back to readSamples2\n");			    /* jwb 02/16/17 */
    for(i=0;i<10;i++) 
    P("sample No = %d, time = %.5f, signal = %.1f\n",	    /* jwb 02/20/17 */
        i, i/(*sr), *(*signal+i));			    /* jwb 02/21/17 */ 
    P("leave readSamples()\n");				    /* jwb 02/22/17 */
  }							    /* jwb 02/22/17 */
  *nsamps = sampN;					    /* jwb 02/16/17 */
} /* end readSamples() */

#include "byteorder.h"
#include "sndhdr.h"
#include "wavhdr.h"
int open(char*, int);					    /* jwb 02/21/17 */
int getfiltype(char*);					    /* jwb 02/21/17 */
int filetype;				    		    /* jwb 02/22/17 */
#define TYPES 2
#define SND 0
#define WAV 1
#define SHORT 1
#define READ 0
char *tail[TYPES] = {"snd", "wav"};

int openSoundFile(int* samplerate, int* Nsamps)		    /* jwb 02/16/17 */
{
  int sr, nchans, samptype, sampsize, sampN;
  int fd; 

  P("In function openSoundFile:\n");			    /* jwb 10/17/16 */
  fd = open(filename, READ);			    	    /* jwb 02/16/17 */
  if(fd == (-1)) 
  {
    fprintf(stderr, "Can not open input file %s\n",	    /* jwb 10/17/16 */
             filename);					    /* jwb 10/17/16 */
    exit(1);						    /* jwb 10/17/16 */
  }
  P("File %s is opened\n", filename);			    /* jwb 02/24/17 */
  filetype = getfiltype(filename);			    /* jwb 02/16/17 */
  if(DIAG) 						    /* jwb 02/22/17 */
  {							    /* jwb 02/22/17 */
    P("filetype = %d\n", filetype);			    /* jwb 02/16/17 */
    P("Read file header\n");				    /* jwb 02/16/17 */
  }							    /* jwb 02/22/17 */
  if(filetype == SND)
  {
    readSndHdr(fd, samplerate, &nchans, Nsamps, &samptype);
    if(DIAG) 						    /* jwb 02/22/17 */
    {							    /* jwb 02/22/17 */
      P("  fd = %d, sr = %d, nchans = %d, *Nsamps = %d\n",  /* jwb 02/22/17 */
        fd, *samplerate, nchans, *Nsamps);
    }							    /* jwb 02/22/17 */
    if (samptype == SND_FORMAT_LINEAR_16)
    {
      filetype = SHORT;
      sampsize = 16;
    }
    else
    {
      fprintf(stderr, 
       "Only 16-bit short int samples in PCM format are supported.\n"); /*jwb*/
      P("\n");
      exit(1);
    }
    /* SND data is big-endian, so if the host is Intel little-endian,
       byte-reverse the data. */
    byte_reverse = (byte_order() == little_endian);	    /* jwb 10/17/16 */
  }
  else if(filetype == WAV)
  {
    readWavHdr(fd, samplerate, &nchans, Nsamps, &sampsize);
    if(DIAG) 						    /* jwb 02/22/17 */
    {							    /* jwb 02/22/17 */
      P("  fd = %d, sr = %d, nchans = %d, *Nsamps = %d\n",  /* jwb 02/22/17 */
        fd, *samplerate, nchans, *Nsamps);
    }							    /* jwb 02/22/17 */
    if(sampsize == 16) filetype = SHORT;
    else
    {
      fprintf(stderr, 
       "Only 16-bit short int samples in PCM format are supported.\n"); /*jwb*/
      P("\n");
      exit(1);
    }
    /* WAVE data is little-endian, so if the host is a Motorala big-endian,
       byte-reverse the data. */
    byte_reverse = (byte_order() == big_endian);
  }
  if(DIAG) 						    /* jwb 02/22/17 */
  {							    /* jwb 02/22/17 */
    if(byte_reverse) P("  byte reverse is on!\n");	    /* jwb 02/21/17 */
    else P("  byte reverse is off!\n");		    	    /* jwb 02/21/17 */
    P("file header now read. ready for sample input\n");    /* jwb 02/16/17 */
    P("leave openSoundFile\n");				    /* jwb 02/16/17 */
  }							    /* jwb 02/22/17 */
  return fd;
} /* end openSoundFile() */

void readsamps(float* signal,int fd,float sr,int sampN)     /* jwb 02/21/17 */ 
{
  int i, ret;						    /* jwb 10/16/16 */
  short int *samps;					    /* jwb 10/17/16/*/

  P("In function readsamps:\n");			    /* jwb 02/21/17 */
  P("Read sound samples\n");				    /* jwb 02/24/17 */
  if(DIAG)						    /* jwb 02/23/17 */
  {							    /* jwb 02/23/17 */
    P("  fd = %d, sr = %.1f, no. samps = %d\n",		    /* jwb 02/21/17 */ 
         fd, sr, sampN);				    /* jwb 02/21/17 */
  }							    /* jwb 02/23/17 */

/* samps is a temporary array to hold the short int samples coming
   from the sound file */
  samps = (short int*)calloc(sampN, sizeof(short int));	    /* jwb 10/17/16/*/

  ret = read(fd, samps, sampN*sizeof(short int));	    /* jwb 10/17/16 */
  if(ret == -1)
  {
    fprintf(stderr,"Can not read samples!\n\n");	    /* jwb 10/18/16 */
    exit(1);
  }
  for(i=0;i<sampN;i++) 					    /* jwb 10/17/16 */
  {
    if(byte_reverse) byteswap2(samps+i);		    /* jwb 10/17/16 */
    signal[i] = samps[i];				    /* jwb 02/20/17 */
    if(DIAG)						    /* jwb 02/23/17 */
    {							    /* jwb 02/23/17 */
      if(i<10)						    /* jwb 10/18/16 */
      {
        P(" samps = %d", samps[i]);
        P("  sample No = %d, time = %.5f, signal = %.1f\n", /* jwb 02/20/17 */
           i, i/sr, signal[i]);                             /* jwb 02/20/17 */
      }
    }
  }
  if(DIAG) P("Leave readsamps\n");			    /* jwb 02/23/17 */
} /* end readsamps */

int getfiltype(char* name)
{
  int i, len;
   len = strlen(name);
   for(i=0;i<TYPES;i++)
  {
    if(!strcmp(&name[len-3],tail[i]))
    	return(i);
  }
  return(TYPES);
} /* getfiltype() */
