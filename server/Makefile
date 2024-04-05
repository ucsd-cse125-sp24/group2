CFLAGS = -g
INCFLAGS = -I./include
LDFLAGS = -L./lib

RM = /bin/rm -f
server.o: src/server.cpp
	$(CC) $(INCFLAGS) -lws2_32 -c src/server.cpp 
clean:
	$(RM) *.o Client
