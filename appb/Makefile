all: timestamp.elf create-file.elf

timestamp.elf: timestamp.o
	gcc $(CFLAGS) -o timestamp.elf timestamp.o

timestamp.o: timestamp.c
	gcc $(CFLAGS) -c timestamp.c

create-file.elf: create-file.o
	gcc $(CFLAGS) -o create-file.elf create-file.o

create-file.o: create-file.c
	gcc $(CFLAGS) -c create-file.c

clean:
	rm -f *.o *.elf *.txt
