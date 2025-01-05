all: client server

client: basic_client.o pipe_networking.o
	gcc -o client basic_client.o pipe_networking.o
	./client

server: persistant_server.o pipe_networking.o
	gcc -o per_server persistant_server.o pipe_networking.o
	./per_server

basic_client.o: basic_client.c pipe_networking.h
	gcc -c basic_client.c

basic_server.o: basic_server.c pipe_networking.h
	gcc -c basic_server.c

persistant_server.o: persistant_server.c pipe_networking.h
	gcc -c persistant_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h
	gcc -c pipe_networking.c

clean:
	rm -f *.o *~ client server per_server
