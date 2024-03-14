CC=gcc
CFLAGS= -Wall

main: main.o dir_listing.o
	$(CC) -o main main.o dir_listing.o $(CFLAGS)

main.o: main.c dir_listing.h
	$(CC) -c main.c $(CFLAGS)

dir_listing.o: dir_listing.c dir_listing.h
	$(CC) -c dir_listing.c $(CFLAGS)

clean:
	rm -f main *.o

