CC=gcc
CFLAGS=-g -Wall -L/usr/local/blcr-0.8.5/lib/ -lcr

all: ckpt-modeloff-bin

ckpt-modeloff-bin: loop.o 
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<
	
clean:
	rm -f *.o ckpt-modeloff-bin