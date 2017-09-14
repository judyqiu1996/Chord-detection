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
void readSamples(float**, float*, int*);
int plotseg(float*,float*,int,char*,char*);
int plabel(double,double);
int notedetect(float* z, int m, float sr,char** chord);
void fft(float** y, float** x, int m,float** z);
char *filename;
char graphlabel[400];

int main(int argc,char** argv)
{
  int i, nsamps,t,m=13;
  float sr, dt;
  float *audio, *x, *y, *z,*fre;
  char* nonchord = {"!"};
  char *chord;

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

  fre = (float*)calloc(pow(2,m),sizeof(float));
  x = (float*)calloc(pow(2,m+1),sizeof(float));

  y = (float*)calloc(pow(2,m+1),sizeof(float));
  z = calloc(pow(2,m),sizeof(float));
  chord = calloc(2,sizeof(char));
  
  for(t=0;t<pow(2,m);t++){
    x[t]=audio[t+10000];
    fre[t] = t/pow(2,m+1)*sr;
  }

  fft(&y,&x,m,&z);
  int check = notedetect(z, m, sr,&chord);
  if(chord==nonchord) printf("Chord can't be recognized!\n");
  else if (check==0) printf("This is %s chord\n", chord);
  else printf("This might be %s chord or chord can't be recognized\n", chord );

  P("Plot samples\n\n");
  plotseg(fre,z, pow(2,10), "frequency (Hz)", "AMPLITUDE");
  free(x);
  free(y);
  free(z);
  free(fre);
  
}  /* end main() */

int plabel(double xpos,double ypos)
{
    g_move_abs(xpos,ypos);
    g_text(graphlabel);
    return(0);
}

