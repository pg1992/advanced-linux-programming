CC=gcc

.PHONY: clean all

all: arglist.elf getopt_long.elf stdio.elf buffer.elf print-env.elf client.elf\
	temp_file.elf tmpfile.elf error_handling.elf readfile.elf

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.elf *.o
