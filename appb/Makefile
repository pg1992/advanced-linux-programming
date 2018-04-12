all: timestamp.elf create-file.elf hexdump.elf lseek-huge.elf

%.elf: %.o
	gcc $(CFLAGS) -o $@ $<

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -f *.o *.elf *.txt
