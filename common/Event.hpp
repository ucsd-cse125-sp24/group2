#ifndef EVENT_H
#define EVENT_H
#include "EventArgs.hpp"
#include <vector>
#include <mutex>

typedef std::function<void(EventArgs*)> Action;
template <typename T> class EventHandler {
    static_assert(std::is_base_of<EventArgs, T>::value,
                  "Only EventArgs is allowed");

private:
    std::mutex m;
    std::vector<Action> callbacks;

public:
    inline void invoke(EventArgs* args) {
        std::lock_guard<std::mutex> lock(m);
        for (const auto& callback : callbacks) {
            callback(args);
        }
    }

    inline void operator+=(Action h) {
        std::lock_guard<std::mutex> lock(m);
        callbacks.push_back(h);
    }

    // TODO override -= for unsubscribe
};

#endif