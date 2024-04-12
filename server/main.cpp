#define TICK_RATE_USEC 33333

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <thread>
#include <windows.h>

#include "NetworkManager.hpp"

using namespace std;
volatile int running = 0;
void tick(void);
int main(int argc, char** argv) {
    running = 1;
    std::thread(tick).detach();

    NetworkManager::instance().init();
}

void tick() {
    // Set realtime scheduling
    /*
    const struct sched_param p = {
        .sched_priority = sched_get_priority_max(SCHED_FIFO)};
    sched_setscheduler(0, SCHED_FIFO, &p);
    */

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
            long t = (long)(chrono::duration_cast<chrono::milliseconds>(time_to_sleep % chrono::seconds(1)).count());
            Sleep(t);
        } else {
            printf("Server is running behind!\n");
        }
    }

    return;
}