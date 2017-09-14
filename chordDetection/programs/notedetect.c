/*************************************************************************
    nodetec.c

    Program example to detect notes in files and recognize as one chord or not a chord

    Author: Yujia Qiu    Date: April 27th,2017

 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "macro.h"


void fft(float** y, float** x, int m,float** z){
	int i;
	float sum,scale = 1/pow(2,m);

	fft2((*x),(*y),m,scale,0);

	 for(i=0;i<pow(2,m);i++){
        sum = pow((*x)[i],2)+pow((*y)[i],2);
		(*z)[i] = sqrt(sum);}
	}

int notedetect(float* z, int m, float sr,char** chord){
	const float fre[88] = {27.5, 29.135, 30.868, 32.703, 34.648, 36.708, 38.891, 41.203, 43.654, 46.249, 48.999, 51.913, 
		55.0, 58.27, 61.735, 65.406, 69.296, 73.416, 77.782, 82.407, 87.307, 92.499, 97.999, 103.826, 110.0, 116.541, 
		123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614, 184.997, 195.998, 207.652, 220.0, 233.082, 
		246.942, 261.626, 277.183, 293.665, 311.127, 329.628, 349.228, 369.994, 391.995, 415.305, 440.0, 466.164, 
		493.883, 523.251, 554.365, 587.33, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880.0, 932.328, 
		987.767, 1046.502, 1108.731, 1174.659, 1244.508, 1318.51, 1396.913, 1479.978, 1567.982, 1661.219, 1760.0, 
		1864.655, 1975.533, 2093.005, 2217.461, 2349.318, 2489.016, 2637.02, 2793.826, 2959.955, 3135.963, 3322.438, 
		3520.0, 3729.31, 3951.066, 4186.009};

	const char *note[88]={"A0", "A#0", "B0", "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#", "A1", "A#1", "B1", "C2",
	 "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2", "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", 
	 "G3", "G#3", "A3", "A#3", "B3", "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4", "C5",
	 "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", 
	  "G6", "G#6", "A6", "A#6", "B6", "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7", "C8"};
	char *nonchord = {"!"};
	char *chordname[12]={"C","C#","D","Eb","E","F","F#","G","Ab","A","Bb","B"};
	const char *chordnote[12][3]={{"C","E","G"},{"C#","F","G#"},{"A","D", "F#"},{"A#","D#","G"},{"B","E", "G#"},
								  {"F", "A", "C"},{"F#", "A#", "C#"},{"G","B","D"},{"G#", "C", "D#"},
								  {"A", "C#", "E"},{"A#", "D", "F"},{"B", "D#", "F#"}};
	const int majorchord[12][3]={{1,5,8},{2,6,9},{3,7,10},{4,8,11},{5,9,12},{1,6,10},{2,7,11},{3,8,12},{1,4,9},{2,5,10},{3,6,11},{4,7,12}};
	
	int row=88,col=pow(2,m),i,j,f0,n;
	int noteindex[3];
	float amp[row];

	for(i=0;i<row;i++){
		f0=fre[i];	
		amp[i]=0;
		n=1;
		while(f0<.5*sr){
			amp[i]+=(1./n)*z[(int)(f0/sr*2*col)];
			f0+=fre[i];
			n++;
		}
	}
	
	float threemax[3];
	float max = 0;
	int index[3];
	for(i=0;i<row;i++){
		if(amp[i]>max){
			max=amp[i];
			index[0]=i;
		}
	}
	threemax[0]=max;
	max=0;
	for(i=0;i<row;i++){
		if(amp[i]>max && amp[i]<threemax[0]){
			max=amp[i];
			index[1]=i;
		}
	}
	threemax[1]=max;
	max=0;
	for(i=0;i<row;i++){
		if(amp[i]>max && amp[i]<threemax[1]){
			max=amp[i];
			index[2]=i;
		}
	}
	threemax[2]=max;
	printf("Three most likely notes in the audio:\n");
	for(i=0;i<3;i++)
		printf("note:%s probability:%f\n",note[index[i]],threemax[i]/threemax[0]); 
	

	for(i=0;i<3;i++){
		if(index[i]<3)
			noteindex[i]=index[i]+9;
		else
			noteindex[i]=(index[i]-3)%12;
	}


	int temp;
	if(noteindex[0]>noteindex[1]){
		temp=noteindex[0];
		noteindex[0]=noteindex[1];
		noteindex[1]=temp;
	}
	if(noteindex[0]>noteindex[2]){
		temp=noteindex[0];
		noteindex[0]=noteindex[2];
		noteindex[2]=temp;
	}
	if(noteindex[1]>noteindex[2]){
		temp=noteindex[1];
		noteindex[1]=noteindex[2];
		noteindex[2]=temp;
	}

	for(i=0;i<3;i++){
		noteindex[i]++;
	}


	int prob[3];
	int minindex,min=100;
	for(i=0;i<12;i++){
		for(j=0;j<3;j++){
			prob[j]=0;
			prob[j]+=min(min(abs(noteindex[j]-majorchord[i][0]),abs(noteindex[j]-majorchord[i][1])),abs(noteindex[j]-majorchord[i][2]));
		}
		if(prob[0]==0&&prob[1]==0&&prob[2]==0){
			*chord= chordname[i];
			return 0;
		}
		else if(prob[0]==0&&prob[1]==0&&(prob[2]<min)){
			min=prob[2];
			minindex=i;
		}
		else if(prob[0]==0&&prob[2]==0&&(prob[1]<min)){
			min=prob[1];
			minindex=i;
		}
		else if(prob[1]==0&&prob[2]==0&&(prob[0]<min)){
			min=prob[1];
			minindex=i;
		}
	
	}
	if(min==100){
		*chord=nonchord;
		return 1;
	}
	*chord = chordname[minindex];
	return 1;

}
