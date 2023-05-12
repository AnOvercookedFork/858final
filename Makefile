CFLAGS = -fopenmp -O2 -g
CC = g++

serial: serial.cpp
	$(CC) $(CFLAGS) -o $@ $<

hld: hld.cpp segment_tree.cpp
	$(CC) $(CFLAGS) -o $@ $^

st_test: st_test.cpp segment_tree.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf serial hld st_test