#define TICK_RATE_USEC 33333

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <pthread.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "NetworkManager.hpp"

using namespace std;
volatile int running = 0;
static void* start_server(void*);
int main(int argc, char** argv) {
    running = 1;

    // Start server
    pthread_t thread;
    pthread_create(&thread, NULL, start_server, NULL);

    // Main game loops
    unsigned long tick = 0;
    auto start_time = chrono::steady_clock::now();
    while (running) {
        chrono::time_point<chrono::steady_clock> new_time = chrono::steady_clock::now();
        tick++;
        chrono::time_point<chrono::steady_clock> next_tick_time = start_time + tick * chrono::microseconds(TICK_RATE_USEC);
        // TODO handle input
        // TODO update game state
        NetworkManager::instance().update();
        // TODO send updated state
        NetworkManager::instance().send_state();

        // Wait for end of tick
        auto time_to_sleep = next_tick_time - chrono::steady_clock::now();
        if (time_to_sleep.count() > 0) {
#ifdef _WIN32
            long t = (long)(chrono::duration_cast<chrono::milliseconds>(time_to_sleep % chrono::seconds(1)).count());
            Sleep(t);
#else
            const struct timespec t = {
                (time_t)(chrono::duration_cast<chrono::seconds>(time_to_sleep).count()),
                (long)(chrono::duration_cast<chrono::nanoseconds>(time_to_sleep % chrono::seconds(1)).count())};
            nanosleep(&t, NULL);
#endif
        } else {
            printf("Server is running behind!\n");
        }
    }

}

void* start_server(void*) {
    NetworkManager::instance().init();
}