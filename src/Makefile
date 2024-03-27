CC = gcc
CFLAGS = -Wall

main: main.o file_utilities.o cryptography.o
	$(CC) -o main main.o file_utilities.o cryptography.o $(CFLAGS)

main.o: main.c file_utilities.h
	$(CC) -c main.c $(CFLAGS)

file_utilities.o: file_utilities.c file_utilities.h
	$(CC) -c file_utilities.c $(CFLAGS)

cryptography.o: cryptography.c cryptography.h
	$(CC) -c cryptography.c $(CFLAGS)

clean:
	rm -f main *.o

