CC = gcc
CFLAGS = -Wall

main: main.o dir_listing.o cryptography.o
	$(CC) -o main main.o dir_listing.o cryptography.o $(CFLAGS)

main.o: main.c dir_listing.h
	$(CC) -c main.c $(CFLAGS)

dir_listing.o: dir_listing.c dir_listing.h
	$(CC) -c dir_listing.c $(CFLAGS)

cryptography.o: cryptography.c cryptography.h
	$(CC) -c cryptography.c $(CFLAGS)

clean:
	rm -f main *.o

