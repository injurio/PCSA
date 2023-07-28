CC=gcc
CXX=g++
CFLAGS=-std=gnu17
CXXFLAGS=-std=gnu++17
LIBFLAGS=-pthread
TARGETS=seq_echo cc_echo_proc cc_echo_thr one_pool

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

seq_echo: seq_echo.o pcsa_net.o echo_logic.o
	$(CC) -o $@ $^

cc_echo_proc:cc_echo_proc.o pcsa_net.o echo_logic.o
	$(CC) -o $@ $^

cc_echo_thr:cc_echo_thr.o pcsa_net.o echo_logic.o
	$(CC) $(LIBFLAGS) -o $@ $^

one_pool: one_pool.cpp
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
