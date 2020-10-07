CC=gcc
FLAGS=-O2 -std=c99 -lpthread

all: server client
	make server
	make client
server: server.c
	${CC} ${FLAGS} -o server server.c 
client: client.c
	${CC} ${FLAGS} -o client client.c
clean:
	rm server client
