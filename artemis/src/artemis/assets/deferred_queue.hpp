#pragma once
#include <cstdint>
#include <functional>
#include <queue>
namespace artemis {
/**
 * A deferred action system used to execute functions in following frames.
 * Primarily used to defer deletion of GPU resources.
 */
class DeferredQueue {
  public:
    DeferredQueue() = default;
    DeferredQueue(uint32_t num_frames);
    ~DeferredQueue();

    /**
     * Flushes the queues of the current frame index,
     * and updates the current frame index to the next frame.
     */
    void flush();

    /**
     * Enqueue an action for a specific frame index.
     * @param func The action.
     * @param index The index of the frame.
     */
    void enqueue(const std::function<void()>& func, uint32_t index);
    /**
     * Enqueue an action for the next time this frame occurs.
     * @param func The action.
     */
    void enqueue(const std::function<void()>& func);

  private:
    std::vector<std::queue<std::function<void()>>> queues_;
    uint32_t frame_index_ = 0;
    const uint32_t num_frames_ = 0;
};
} // namespace artemis
