create-file.elf: create-file.o
	gcc $(CFLAGS) -o create-file.elf create-file.o

create-file.o: create-file.c
	gcc $(CFLAGS) -c create-file.c

clean:
	rm -f *.o *.elf
