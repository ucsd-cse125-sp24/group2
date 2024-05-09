#pragma once
#include <mutex>
#include <deque>

template <typename T> class ConcurrentQueue {
private:
    std::mutex _mutex;
    std::deque<T> deque;

public:
    T pop_front() {
        std::lock_guard<std::mutex> lock(_mutex);
        T ret = deque.front();
        deque.pop_front();
        return ret;
    }

    void push_back(T val) {
        std::lock_guard<std::mutex> lock(_mutex);
        deque.push_back(val);
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(_mutex);
        return deque.empty();
    }
};