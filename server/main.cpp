#define TICK_RATE_USEC 33333

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <chrono>
#ifdef _WIN32
#include <windows.h>
#endif

#include "NetworkManager.hpp"

using namespace std;
volatile int running = 0;
int main(int argc, char** argv) {
    running = 1;

    // Start server
    NetworkManager::instance().init();

    // Main game loops
    unsigned long tick = 0;
    auto start_time = chrono::steady_clock::now();
    while (running) {
        chrono::time_point<chrono::steady_clock> new_time =
            chrono::steady_clock::now();
        tick++;
        chrono::time_point<chrono::steady_clock> next_tick_time =
            start_time + tick * chrono::microseconds(TICK_RATE_USEC);
        // TODO handle input
        NetworkManager::instance().process_input();
        NetworkManager::instance().update();
        NetworkManager::instance().send_state();

        // Wait for end of tick
        auto time_to_sleep = next_tick_time - chrono::steady_clock::now();
        if (time_to_sleep.count() > 0) {
#ifdef _WIN32
            long t = (long)(chrono::duration_cast<chrono::milliseconds>(
                                time_to_sleep % chrono::seconds(1))
                                .count());
            Sleep(t);
#else
            const struct timespec t = {
                (time_t)(chrono::duration_cast<chrono::seconds>(time_to_sleep)
                             .count()),
                (long)(chrono::duration_cast<chrono::nanoseconds>(
                           time_to_sleep % chrono::seconds(1))
                           .count())};
            nanosleep(&t, NULL);
#endif
        } else {
            printf("Can't keep up! Running %llu tick(s) behind.\n",
                   (long long)ceil(abs(time_to_sleep.count()) /
                                   (double)(TICK_RATE_USEC * 1000)));
        }
    }
}