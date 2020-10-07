CC=gcc
FLAGS=-g -O0

all: server client
	make server
	make client
server: server.c
	${CC} -o server server.c ${FLAGS}
client: client.c
	${CC} -o client client.c ${FLAGS}
clean:
	rm server client
