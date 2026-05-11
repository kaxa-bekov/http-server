CC = gcc
CFLAGS = -Wall -Wextra -g


server:
	$(CC) $(CFLAGS) main.c http_parser.c http_init_tcp.c file_manipulation.c -o server

main.o: main.c
	$(CC) $(CFLAGS) -c main.c
