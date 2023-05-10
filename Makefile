CFLAGS = -fopenmp -O2 -g
CC = g++

serial: serial.cpp
	$(CC) $(CFLAGS) -o $@ $<

serial2: serial2.cpp
	$(CC) $(CFLAGS) -o $@ $<

serial3: serial3.cpp
	$(CC) $(CFLAGS) -o $@ $<
