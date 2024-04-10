ifeq ($(CC),x86_64-w64-mingw32-g++)
CFLAGS = -g -static-libgcc -static-libstdc++ -lws2_32 -pthread --static
else
CFLAGS = -g -std=c++11
endif
INCFLAGS = -I./include
LDFLAGS = -L./lib

RM = /bin/rm -f
Server: main.o server.o game_manager.o client.o
	$(CC) -o Server client.o server.o main.o game_manager.o $(LDFLAGS) $(CFLAGS) $(INCFLAGS)
main.o: main.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c main.cpp
server.o: src/server.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c src/server.cpp 
game_manager.o: src/game_manager.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c src/game_manager.cpp
client.o: src/client.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c src/client.cpp
clean:
	$(RM) *.o Server.exe
