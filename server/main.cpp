#define TICK_RATE_USEC 33333

#include "server.hpp"
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <chrono>
#include <sched.h>
#include <pthread.h>

using namespace std;
volatile int running = 0;
void* tick(void*);
int main(int argc, char** argv) {
    Server server;
    pthread_t main_thread;

    running = 1;
    int res = pthread_create(&main_thread, NULL, tick, NULL);
    if (res != 0) {
        perror("Failed to create thread");
        return 1;
    }

    server.init();
}

void* tick(void* params) {
    // Set realtime scheduling
    const struct sched_param p = {
        .sched_priority = sched_get_priority_max(SCHED_FIFO)};
#ifdef __APPLE__
    setpriority(PRIO_PROCESS, 0, PRIO_MIN);
#else
    sched_setscheduler(0, SCHED_FIFO, &p);
#endif

    unsigned long tick = 0;

    auto start_time = chrono::steady_clock::now();
    while (running) {
        chrono::time_point<chrono::steady_clock> new_time = chrono::steady_clock::now();
        tick++;
        chrono::time_point<chrono::steady_clock> next_tick_time = start_time + tick * chrono::microseconds(TICK_RATE_USEC);
        // TODO handle input
        // TODO update game state
        // TODO send updated state
        printf("Tick %lu\n", tick);

        // Wait for end of tick
        auto time_to_sleep = next_tick_time - chrono::steady_clock::now();
        if (time_to_sleep.count() > 0) {
            const struct timespec t = {
                (time_t)(chrono::duration_cast<chrono::seconds>(time_to_sleep).count()),
                (long)(chrono::duration_cast<chrono::nanoseconds>(time_to_sleep % chrono::seconds(1)).count())};
            nanosleep(&t, NULL);
        } else {
            printf("Server is running behind!\n");
        }
    }

    return 0;
}