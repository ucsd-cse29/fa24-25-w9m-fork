all: grade-server

grade-server: grade-server.c http-server.c http-server.h
	gcc -g -o grade-server grade-server.c http-server.c --std=c11 -Wall -Wno-unused-variable

clean:
	rm -f chat-server chat-client
