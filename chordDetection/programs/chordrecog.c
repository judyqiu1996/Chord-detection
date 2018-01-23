/*************************************************************************
        plotSamples.c

     Program example to read and plot sound file samples

 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "g_raph.h"
#include "macro.h"
#define P printf
#define DIAG 0
#define pi acos(-1.0)
void readSamples(float**, float*, int*);
int plotseg(float*,float*,int,char*,char*);
int plabel(double,double);
void notedetect(float*, int, float, char**, char**);
void fft(float**, float** , int,float**);
char *filename;
char graphlabel[400];

int main(int argc,char** argv)
{
  int i, nsamps,t,m=14, len = pow(2,m);
  float sr, dt;
  float *audio, *x, *y, *z,*fre, *db, pret;
  char *chord, *type, *preChord, *preType;


  P("Program to read and plot sound file samples \n");
  if(argc != 2)
  {
    fprintf(stderr,"Useage: %s <filename>\n\n", argv[0]);
    exit(1);
  }
  filename = argv[1];
  readSamples(&audio, &sr, &nsamps);
  P("sample rate = %.0f, no. samples = %d\n", sr, nsamps);
  if(DIAG)
  {
    for(i=0;i<10;i++)
    P("sample No = %d, time = %.5f, signal = %.1f\n",
       i,i/sr,audio[i]);
  }
  sprintf(graphlabel,"Spectrum Plot of file %s",filename);

  fre = (float*)calloc(len,sizeof(float));
  x = (float*)calloc(2*len,sizeof(float));
  y = (float*)calloc(2*len,sizeof(float));
  z = calloc(len,sizeof(float));
  // db = calloc(len,sizeof(float));
  
  for(i=0;i<nsamps;i+=sr/3){
    for(t=0;t<len;t++){
      x[t] = audio[t+i];
      //hamming window
      // x[t] *= .54 + .46*cos(2*pi*(t-len/2.)/(len-1)); 
      //Blackman–Harris window
      x[t] *= 0.35875 - 0.48829*cos(2*pi*t/(len-1)) + 0.14128*cos(4*pi*t/(len-1)) - 0.01168*cos(6*pi*t/(len-1)); 
      fre[t] = t/(2.*len)*sr;
    }

    fft(&y,&x,m,&z);
    notedetect(z, m, sr, &chord, &type);
    if(preChord!=chord){
      if(i!=0) printf("Time: %.2f-%.2fs, Chord: %s %s\n", pret/(sr*1.0), i/(sr*1.0), preChord, preType);
      preChord=chord;
      preType=type;
      pret=i;
    }
  }

  /* db verison
  for(t=0;t<len;t++){
  	if(z[t]>0)
  		db[t] = 20*log10(z[t]);
  	if(z[t]<40.) z[t] = 0.;
  }
  */ 

  P("Plot samples\n\n");
  // plotseg(fre,z, pow(2,11), "frequency (Hz)", "AMPLITUDE");
  // plotseg(fre,db,pow(2,10), "frequency (Hz)", "Amplitude(dB)");
  free(x);
  free(y);
  free(z);
  free(fre);
  // free(db);
  
}  /* end main() */

int plabel(double xpos,double ypos)
{
    g_move_abs(xpos,ypos);
    g_text(graphlabel);
    return(0);
}

