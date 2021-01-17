CC=gcc
FLAGS=-O2 -std=c99 -lpthread
# FLAGS=-O0 -g -std=c99 -lpthread

all: SCM_Server SCM_Client
	make SCM_Server
	make SCM_Client
SCM_Server: SCM_Server.c
	${CC} ${FLAGS} -o SCM_Server SCM_Server.c 
SCM_Client: SCM_Client.c
	${CC} ${FLAGS} -o SCM_Client SCM_Client.c
clean:
	rm SCM_Server SCM_Client
