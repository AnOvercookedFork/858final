CFLAGS = -fopenmp -O2 
CC = g++

serial: serial.cpp
	$(CC) $(CFLAGS) -o $@ $<