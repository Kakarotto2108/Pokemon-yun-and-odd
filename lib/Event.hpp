#pragma once
#include <functional>
#include <vector>

template<typename... Args>
class Event {
public:
    using Listener = std::function<void(Args...)>;

    void subscribe(Listener listener) {
        listeners.push_back(listener);
    }

    void notify(Args... args) {
        for (auto& l : listeners) {
            l(args...);
        }
    }

private:
    std::vector<Listener> listeners;
};

