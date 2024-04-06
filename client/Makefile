CFLAGS = -g
INCFLAGS = -I./include
LDFLAGS = -L./lib

RM = /bin/rm -f
Client: main.o client.o
	$(CC) -o Client client.o main.o $(LDFLAGS) -lws2_32 -static-libgcc -static-libstdc++
main.o: main.cpp
	$(CC) $(INCFLAGS) -c main.cpp
client.o: src/client.cpp
	$(CC) $(INCFLAGS) -lws2_32 -c src/client.cpp 
clean:
	$(RM) *.o Client.exe
