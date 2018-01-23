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

const char *octive[12] = {"C","C#","D","Eb","E","F","F#","G","Ab","A","A#","B"};


char *triadChordName[12]={"C","C#","D","Eb","E","F","F#","G","Ab","A","Bb","B"};
const char *majorChordNote[12][3]={{"C","E","G"}, {"C#","F","G#"}, {"A","D", "F#"}, {"A#","D#","G"},
							  	   {"B","E","G#"}, {"F","A","C"}, {"F#","A#","C#"}, {"G","B","D"},
							 	   {"G#","C","D#"}, {"A","C#","E"}, {"A#","D","F"}, {"B","D#","F#"}};
const int majorChord[12][3]={{1,5,8},{2,6,9},{3,7,10},{4,8,11},{5,9,12},{1,6,10},{2,7,11},{3,8,12},{1,4,9},{2,5,10},{3,6,11},{4,7,12}};

const char *minorChordNote[12][3]={{"C","Eb","G"}, {"C#","E","G#"}, {"D","F","A"}, {"Eb","Gb","Bb"},
									{"E","G","B"}, {"F","Ab","C"}, {"F#","A","C#"}, {"G","Bb","D"},
									{"Ab","B","Eb"}, {"A","C","E"}, {"Bb","Db","F"}, {"B","D","F#"}};
const int minorChord[12][3]={{1,4,8},{2,5,9},{3,6,10},{4,7,11},{5,8,12},{1,6,9},{2,7,10},{3,8,11},{4,9,12},{1,5,10},{2,6,11},{3,7,12}};

const char *diminiChordNote[12][3]={{"C","Eb","Gb"}, {"C#","E","G"}, {"D","F","Ab"}, {"D#","F#","A"},
							       {"E","G","Bb"}, {"F","Ab","B"}, {"F#","A","C"}, {"G","Bb","Db"},
							   	   {"G#","B","D"}, {"A","C","Eb"}, {"A#","C#","E"}, {"B","D","F"}};  
const int diminiChord[12][3]={{1,4,7},{2,5,8},{3,6,9},{4,7,10},{5,8,11},{6,9,12},{1,7,10},{2,8,11},{3,9,12},{1,4,10},{2,5,11},{3,6,12}};

const char *augChordNote[12][3]={{"C","E","G#"}, {"C#","F","A"}, {"D","F#","A#"}, {"D#","G","B"},
							 {"E","G#","C"}, {"F","A","C#"}, {"F#","A#","D"}, {"G","B","D#"},
							 {"G#","C","E"}, {"A","C#","F"}, {"A#","D","F#"}, {"B","D#","G"}};
const int augChord[12][3]={{1,5,9},{2,6,10},{3,7,11},{4,8,12},{1,5,9},{2,6,10},{3,7,11},{4,8,12},{1,5,9},{2,6,10},{3,7,11},{4,8,12}};

const char *aug7ChordNote[12][4]={{"C","E","G#","Bb"}, {"C#","F","A","B"}, {"D","F#","A#","C"},
								 {"D#","G","B","Db"}, {"E","G#","C","D"}, {"F","A","C#","Eb"},
								 {"F#","A#","D","E"}, {"G","B","D#","F"}, {"G#","C","E","F#"},
								 {"A","C#","F","G"}, {"A#","D","F#","G"}, {"B","D#","G","A"}};
const int aug7Chord[12][4]={{1,5,9,11},{2,6,10,12},{1,3,7,11},{2,4,8,12},{1,3,5,9},{2,4,6,10},
							{3,5,7,11},{4,6,8,12},{1,5,7,9},{2,6,8,10},{3,7,9,11},{4,8,10,12}};

char *SevenChordName[12] = {"A", "Ab", "B", "Bb", "C", "C#", "D", "Eb", "E", "F", "F#", "G"};
const char *maj7ChordNote[12][4] = {{"A","C#","E","G#"}, {"Ab","C","Eb","G"}, {"B","D#","F#","A#"},
									{"Bb","D","F","A"}, {"C","E","G","B"}, {"C#","F","G#","C"},
									{"D","F#","A","C#"}, {"Eb","G","Bb","D"}, {"E","G#","B","D#"},
									{"F","A","C","E"}, {"F#","A#","C#","F"}, {"G","B","D","F#"}};
const int maj7Chord[12][4]={{2,5,9,10},{1,4,8,9},{4,7,11,12},{3,6,10,11},{1,5,8,12},{1,2,6,9},
							{2,3,7,10},{3,4,8,11},{4,5,9,12},{1,5,6,10},{2,6,7,11},{3,7,8,12}};

const char *dom7ChordNote[12][4] = {{"A","Db","E","Gb"}, {"Ab","C","Eb","Gb"}, {"B","D#","F#","A"},
									{"Bb","D","F","Ab"}, {"C","E","G","Bb"}, {"C#","F","G","B"},
									{"D","F#","A","C"}, {"E","G#","B","D"}, {"Eb","G","Bb","Db"},
									{"F","A","C","Eb"}, {"F#","A#","C#","E"}, {"G","B","D","F"}};

const int dom7Chord[12][4] = {{2,5,7,10},{1,4,7,9},{4,7,10,12},{3,6,9,11},{1,5,8,11},{2,5,8,12},
							  {1,3,7,10},{3,5,9,12},{2,4,8,11},{1,4,6,10},{2,5,7,11},{3,6,8,12}};


void fft(float** y, float** x, int m,float** z){
	int i;
	float sum,scale = 1/pow(2,m);

	fft2((*x),(*y),m,scale,0);

	 for(i=0;i<pow(2,m);i++){
        sum = pow((*x)[i],2)+pow((*y)[i],2);
		(*z)[i] = sqrt(sum);
	}
}

int compare(const void *a, const void *b){
	int *x = (int *) a;
	int *y = (int *) b;
	return *x - *y;
}

void notedetect(float* z, int m, float sr, char** chord, char** type){
	int row=88,col=pow(2,m+1),i,j,f0,n;
	float amp[row], sum=0;

	for(i=0;i<row;i++){
		f0=fre[i];	
		amp[i]=0;
		n=1;
		while(f0<.5*sr){
			amp[i]+=(1./pow(n,4))*z[(int)(f0/sr*col)];
			f0+=fre[i];
			n++;
		}
		sum+=amp[i];
	}

	float average = sum/row*4;
	
	int count = 0, noteIndex[88], thread=0;

	for(i=0;i<row;i++){
		if(amp[i]>average && (fre[i]<4*thread || thread==0)){
			if(thread==0) thread = fre[i];
			if(i<3) noteIndex[count] = i+10;
			else noteIndex[count]=(i-3)%12+1;
			count++;
			// printf("note: %s, frequency: %f,  amplitude: %f average: %f\n", note[i], fre[i], amp[i], average);
		}
	}

	qsort(noteIndex, count,sizeof(count),compare);

	int posNote[4], nth = 0;
	posNote[nth] = noteIndex[0];
	for(i=1;i<count;i++){
		if(posNote[nth] != noteIndex[i]){
			nth++;
			posNote[nth] = noteIndex[i];
			if(nth==3) break;
		}
	}
	if(!(posNote[3]<13 && posNote[3]>0)) posNote[3] = 100;

	qsort(posNote, nth+1, sizeof(nth+1), compare);

	printf("Possible notes in the audio: %s %s %s %s\n", octive[posNote[0]-1],octive[posNote[1]-1],octive[posNote[2]-1],octive[posNote[3]-1]);

	for(i=0;i<12;i++){
		// printf("%d %d %d\n", diminiChord[i][0]-posNote[0], diminiChord[i][1]-posNote[1], diminiChord[i][2]-posNote[2]);
		if((maj7Chord[i][0]-posNote[0])==0 && (maj7Chord[i][1]-posNote[1])==0 && (maj7Chord[i][2]-posNote[2])==0 && (maj7Chord[i][3]-posNote[3])==0){
			*chord = SevenChordName[i];
			*type = "Major Seventh";
			break;
		}
		else if((dom7Chord[i][0]-posNote[0])==0 && (dom7Chord[i][1]-posNote[1])==0 && (dom7Chord[i][2]-posNote[2])==0 && (dom7Chord[i][3]-posNote[3])==0){
			*chord = SevenChordName[i];
			*type = "Dominent Seventh";
			break;
		}
		else if((aug7Chord[i][0]-posNote[0])==0 && (aug7Chord[i][1]-posNote[1])==0 && (aug7Chord[i][2]-posNote[2])==0 && (aug7Chord[i][3]-posNote[3])==0){
			*chord = triadChordName[i];
			*type = "Augmented Seventh";
			break;
		}
		else if((majorChord[i][0]-posNote[0])==0 && (majorChord[i][1]-posNote[1])==0 && (majorChord[i][2]-posNote[2])==0){
			*chord = triadChordName[i];
			*type = "Major";
			break;
		}	
		else if((minorChord[i][0]-posNote[0])==0 && (minorChord[i][1]-posNote[1])==0 && (minorChord[i][2]-posNote[2])==0){
			*chord = triadChordName[i];
			*type = "Minor";
			break;
		}
		else if((diminiChord[i][0]-posNote[0])==0 && (diminiChord[i][1]-posNote[1])==0 && (diminiChord[i][2]-posNote[2])==0){
			*chord = triadChordName[i];
			*type = "Diminished";
			break;
		}
		else if((augChord[i][0]-posNote[0])==0 && (augChord[i][1]-posNote[1])==0 && (augChord[i][2]-posNote[2])==0){
			*chord = triadChordName[i];
			*type = "Augmented";
			break;
		}
		else{
			*chord = "None";
			*type = "";
		}
	}
}
