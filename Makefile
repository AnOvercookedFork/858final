CFLAGS = -fopenmp -O2 -g
CC = g++

serial_hld: serial_hld.cpp serial_st.cpp tree.cpp
	$(CC) $(CFLAGS) -o $@ $^

serial_st_test: serial_st_test.cpp serial_st.cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf serial hld st_test