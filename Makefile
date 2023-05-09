CFLAGS = -O2
CC = gcc

serial: serial.cpp
	$(CC) $(CFLAGS) -o $@ $<