CC=gcc
CFLAGS=-std=gnu17
TARGETS=tiny

all: $(TARGETS)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

tiny: tiny.o pcsa_net.o
	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
