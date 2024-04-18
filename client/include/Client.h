#ifndef CLIENT_H
#define CLIENT_H

#include "Mover.h"
#include "include/psocket.hpp"

class Client {
   private:
   public:
    Socket psocket;
    Mover* m;
    void init(Mover*);
    static void* receive(void*);
    void send(const char*, int);
};
#endif  // CLIENT_H