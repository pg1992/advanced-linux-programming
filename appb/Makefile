all: timestamp.elf create-file.elf

%.elf: %.o
	gcc $(CFLAGS) -o $@ $<

%.o: %.c
	gcc $(CFLAGS) -c $<

clean:
	rm -f *.o *.elf *.txt
