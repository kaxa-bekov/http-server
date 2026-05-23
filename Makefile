#Declare variables
CC = gcc
CFLAGS = -Wall -Wextra -g
HFILES = http_parser.h http_init_tcp.h file_manipulations.h
OFILES = http_parser.o http_init_tcp.o file_manipulations.o 

#Declare phony targets
.PHONY: all clean

# 'all' is at the top so it runs first by default
all: server

server: main.o $(OFILES)
	$(CC) $(CFLAGS) main.o $(OFILES) -o server

main.o: main.c $(HFILES)
	$(CC) $(CFLAGS) -c main.c

http_init_tcp.o: http_init_tcp.c http_init_tcp.h
	$(CC) $(CFLAGS) -c http_init_tcp.c


http_parser.o: http_parser.c http_parser.h
	$(CC) $(CFLAGS) -c http_parser.c

file_manipulations.o: file_manipulations.c file_manipulations.h
	$(CC) $(CFLAGS) -c file_manipulations.c

#Cleaning
clean:
	rm -f *.o server
