#pragma once
#include "artemis/events/event_listener.hpp"
#include "event.hpp"

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <vector>
namespace artemis {
class EventBus {
  public:
    template <typename T> void subscribe(std::function<void(T&)> callback) {
        // Use type-erasure to push back a lambda that then calls the callback,
        // and casts the base Event type to the real event.
        listeners_[typeid(T)].push_back(
            [cb = std::move(callback)](Event& e) { cb(static_cast<T&>(e)); });
    }

    template <typename T> void publish(T&& event) {
        event_queue_.push(std::make_unique<T>(std::move(event)));
    }

    void flush();

  private:
    std::unordered_map<std::type_index,
                       std::vector<std::function<void(Event&)>>>
        listeners_;
    std::queue<std::unique_ptr<Event>> event_queue_;
};
} // namespace artemis
