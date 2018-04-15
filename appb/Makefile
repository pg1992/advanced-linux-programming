TARGETS=timestamp.elf create-file.elf hexdump.elf lseek-huge.elf read-file.elf\
		write-all.elf write-args.elf gnu-c.elf

all: $(TARGETS)

%.elf: %.o
	gcc $(CFLAGS) -o $@ $<

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -rf *.o *.elf *.txt *.d
