#pragma once
#include "event.hpp"

#include <functional>
#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>
#include <vector>
namespace artemis {
/**
 * Handles the subscription and dispatching of arbritrary events.
 */
class EventBus {
  public:
    /**
     * Subscribe to an event of some type.
     * @param callback The function called with the published events.
     */
    template <typename T>
    void subscribe(std::function<void(const T&)> callback) {
        // Use type-erasure to push back a lambda that then calls the callback,
        // and casts the base Event type to the real event.
        listeners_[typeid(T)].push_back(
            [cb = std::move(callback)](const Event& e) {
                cb(static_cast<const T&>(e));
            });
    }

    /**
     * Publishes a given event of some type.
     * @param event The event.
     */
    template <typename T> void publish(T event) {
        event_queue_.push(std::make_unique<T>(event));
    }

    /**
     * Flushes all events, and calls the relevant subscriber callbacks.
     */
    void flush();

  private:
    std::unordered_map<std::type_index,
                       std::vector<std::function<void(const Event&)>>>
        listeners_;
    std::queue<std::unique_ptr<Event>> event_queue_;
};
} // namespace artemis
