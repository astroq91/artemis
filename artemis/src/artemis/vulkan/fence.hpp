#pragma once

#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {
class Fence {
  public:
    Fence() = default;
    ~Fence() {
        if (fence_ != nullptr) {
            deferred_queue_->enqueue([dev = device_, fence = fence_]() {
                dev->destroyFence(fence);
            });
        }
    }
    Fence(VulkanContext* context, DeferredQueue* deferred_queue)
        : device_(context->device.get()), deferred_queue_(deferred_queue) {
        fence_ = device_->createFence(
            vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
    }

    vk::Result wait(uint64_t timeout) {
        return device_->waitForFences({fence_}, vk::True, timeout);
    }
    void reset() { device_->resetFences({fence_}); }

    vk::Fence& get_vk_fence() { return fence_; }

  private:
    vk::Fence fence_{nullptr};
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
};
} // namespace artemis
