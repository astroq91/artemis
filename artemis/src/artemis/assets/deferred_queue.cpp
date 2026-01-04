#include "deferred_queue.hpp"
#include <stdexcept>

namespace artemis {
DeferredQueue::DeferredQueue(uint32_t num_frames)
    : queues_(num_frames), num_frames_(num_frames) {}

DeferredQueue::~DeferredQueue() {
    for (auto& queue : queues_) {
        while (!queue.empty()) {
            auto& func = queue.front();
            func();
            queue.pop();
        }
    }
}
void DeferredQueue::flush() {
    while (!queues_[frame_index_].empty()) {
        auto& func = queues_[frame_index_].front();
        func();
        queues_[frame_index_].pop();
    }
}

void DeferredQueue::enqueue(const std::function<void()>& func, uint32_t index) {
    if (index >= num_frames_) {
        throw std::runtime_error("Tried to enqueue a deferred action for a "
                                 "frame outside the possible space.");
    }
    queues_[index].push(func);
}
void DeferredQueue::enqueue(const std::function<void()>& func) {
    queues_[frame_index_].push(func);
}

} // namespace artemis
