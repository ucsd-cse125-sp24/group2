ifeq ($(CC),x86_64-w64-mingw32-g++)
CFLAGS = -g -static-libgcc -static-libstdc++ -lws2_32
else
CFLAGS = -g -std=c++11
endif
INCFLAGS = -I./include
LDFLAGS = -L./lib

RM = /bin/rm -f
Client: main.o client.o
	$(CC) -o Client client.o main.o $(LDFLAGS) $(CFLAGS)
main.o: main.cpp
	$(CC) $(INCFLAGS) -c main.cpp
client.o: src/client.cpp
	$(CC) $(INCFLAGS) -c src/client.cpp $(CFLAGS)
clean:
	$(RM) *.o Client.exe
