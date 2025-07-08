CC = gcc
CFLAGS =
WFLAGS = -Wall -Werror
crc32-objs = crc32.o

all: crc32

%.o: %.c
	$(CC) $(CFLAGS) $(WFLAGS) -c -o $@ $<

padjffs2: $(crc32-objs)
	$(CC) $(LDFLAGS) -o $@ $(crc32-objs)

clean:
	rm -f crc32 *.o
