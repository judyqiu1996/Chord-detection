#		Makefile for prog1

OBJS = sndhdr.o wavhdr.o byteorder.o plotseg.o fft2.o g_raph.a
OBJS1 = readSamples.o notedetect.o
INCL = byteorder.h sndhdr.h wavhdr.h macro.h g_raph.h
CC = cc -m32 -c
CCL = cc -m32 -o


plotSamples: chordrecog.c $(OBJS) $(OBJS1) $(INCL)
	$(CCL) chordrecog chordrecog.c $(OBJS) $(OBJS1) -lm

readSamples.o: readSamples.c $(OBJS) $(INCL)
	cc -m32 -c readSamples.c

notedetect.o: notedetect.c $(INCL)
	cc -m32 -c notedetect.c
