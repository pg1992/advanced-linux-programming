CC=gcc

.PHONY: clean all

all: arglist.elf

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -Rf *.elf *.o
