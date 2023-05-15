ifdef CLANG
CC = clang++
else
CC = g++
endif

CPPFLAGS = -std=c++17 -Wall -Werror -I../

ifdef CILKPLUS
CPPFLAGS += -DPARLAY_CILKPLUS -DCILK -fcilkplus
else ifdef SERIAL
CPPFLAGS += -DPARLAY_SEQUENTIAL
else
CPPFLAGS += -pthread
endif

ifdef DEBUG
CPPFLAGS += -Og -mcx16 -DDEBUG
else ifdef PERF
CPPFLAGS += -Og -mcx16 -march=native -g
else ifdef MEMCHECK
CPPFLAGS += -Og -mcx16 -DPARLAY_SEQUENTIAL
else ifdef MXMAC
CPPFLAGS += -O3 -arch x86_64
else
CPPFLAGS += -O3 -mcx16 -march=native
endif

serial_hld: serial_hld.cpp serial_st.cpp tree.cpp
	$(CC) $(CPPFLAGS) -o $@ $^

serial_st_test: serial_st_test.cpp serial_st.cpp
	$(CC) $(CPPFLAGS) -o $@ $^

parlay_test: parlay_test.cpp
	$(CC) $(CPPFLAGS) -o $@ $^

parallel_hld: parallel_hld.cpp serial_st.cpp tree.cpp
	$(CC) $(CPPFLAGS) -o $@ $^	

clean:
	rm -rf serial hld st_test parlay_test