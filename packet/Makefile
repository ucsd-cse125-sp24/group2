ifeq ($(CC),x86_64-w64-mingw32-g++)
CFLAGS = -g -static-libgcc -static-libstdc++ -lws2_32
else
CFLAGS = -g -std=c++11 -lc++
endif
INCFLAGS = -I./include
LDFLAGS = -L./lib

RM = /bin/rm -f
Packet: main.o packet.o
	$(CC) -o Packet packet.o main.o $(LDFLAGS) $(CFLAGS)
main.o: main.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c main.cpp
packet.o: src/packet.cpp
	$(CC) $(INCFLAGS) $(CFLAGS) -c src/packet.cpp 
clean:
	$(RM) *.o Packet.exe
