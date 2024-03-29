build: 
	gcc local_recv.c -o local_recv
	gcc local_send.c -o local_send

all: build
	./local_recv & ./local_send
