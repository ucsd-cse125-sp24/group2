CFLAGS = -g
SERVERINCFLAGS = -I./server/include

RM = /bin/rm -f
client:
	$(CC) client/client.cpp
server: server/src/server.cpp
	$(CC) $(SERVERINCFLAGS) server/src/server.cpp -lws2_32
clean:
	$(RM) *.o Client