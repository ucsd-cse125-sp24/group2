#ifndef EVENT_H
#define EVENT_H
#include "EventArgs.hpp"
#include <algorithm>
#include <vector>
#include <mutex>
#include <algorithm>
#include <functional>

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

    // If a handler tries to remove itself while being invoked, everything will
    // crash due to the mutex
    inline void operator-=(const Action& h) {
        std::lock_guard<std::mutex> lock(m);
        // std::find(callbacks.begin(), callbacks.end(), h);
        auto it = std::find_if(callbacks.begin(), callbacks.end(),
                               [&](const Action& func) {
                                   return func.target<void(EventArgs*)>() ==
                                          h.target<void(EventArgs*)>();
                               });

        if (it != callbacks.end()) {
            callbacks.erase(it);
        }
    }
};

#endif