CC=gcc

.PHONY: clean all

all: arglist.elf getopt_long.elf stdio.elf buffer.elf print-env.elf client.elf\
	temp_file.elf tmpfile.elf error_handling.elf readfile.elf app-static.elf\
	app-dynamic.elf tifftest.elf

tifftest.elf: tifftest.o
	$(CC) $(CFLAGS) -ltiff -o $@ $<

app-static.elf: app.o libtest.a
	$(CC) $(CFLAGS) -static app.o -L. -ltest -o $@

libtest.a: test.o
	ar cr $@ $<

app-dynamic.elf: app.o libtest.so
	$(CC) $(CFLAGS) app.o -L. -ltest -Wl,-rpath,./ -o $@

libtest.so: test.o
	$(CC) $(CFLAGS) -shared -fPIC -o $@ $<

test.o: test.c
	$(CC) $(CFLAGS) -fPIC -c $<

%.elf: %.o
	$(CC) $(CFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.elf *.o *.a *.so
