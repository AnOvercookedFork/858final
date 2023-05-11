CFLAGS = -fopenmp -O2 -g
CC = g++

serial: serial.cpp
	$(CC) $(CFLAGS) -o $@ $<

hld: hld.cpp segment_tree.cpp
	$(CC) $(CFLAGS) hld.cpp segment_tree.cpp -o hld
