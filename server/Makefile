CFLAGS = -g
INCFLAGS = -I./include
LDFLAGS = -L./lib

RM = /bin/rm -f
Server: main.o server.o
	$(CC) -o Server server.o main.o $(LDFLAGS) -lws2_32 -static-libgcc -static-libstdc++
main.o: main.cpp
	$(CC) $(INCFLAGS) -c main.cpp
server.o: src/server.cpp
	$(CC) $(INCFLAGS) -lws2_32 -c src/server.cpp 
clean:
	$(RM) *.o Server.exe
