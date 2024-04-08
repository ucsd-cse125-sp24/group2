ifeq ($(CC),x86_64-w64-mingw32-g++)
CFLAGS = -g -static-libgcc -static-libstdc++ -lws2_32
else
CFLAGS = -g -std=c++11
endif
INCFLAGS = -I./include
LDFLAGS = -L./lib

RM = /bin/rm -f
Server: main.o server.o
	$(CC) -o Server server.o main.o $(LDFLAGS) $(CFLAGS)
main.o: main.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c main.cpp
server.o: src/server.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c src/server.cpp 
clean:
	$(RM) *.o Server.exe
