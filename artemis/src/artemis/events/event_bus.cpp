#include "event_bus.hpp"

namespace artemis {

void EventBus::flush() {
    while (!event_queue_.empty()) {
        auto& event = *event_queue_.front().get();
        auto it = listeners_.find(typeid(event));
        if (it != listeners_.end()) {
            for (auto& cb : it->second) {
                cb(event);
            }
        }
        event_queue_.pop();
    }
}

} // namespace artemis
