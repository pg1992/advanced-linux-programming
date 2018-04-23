CC=gcc

.PHONY: clean all

all: arglist.elf getopt_long.elf stdio.elf

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -Rf *.elf *.o
