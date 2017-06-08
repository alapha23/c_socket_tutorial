#makefile for chatroom
all: client server
client: client_test.c
	gcc -o client client_test.c
server: server_test.c
	gcc -o server server_test.c
clean:
	rm client server
