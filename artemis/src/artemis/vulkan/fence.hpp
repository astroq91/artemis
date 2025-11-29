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
    Fence(const vk::FenceCreateFlags& flags, VulkanContext& context,
          DeferredQueue* deferred_queue)
        : device_(context.device.get()), deferred_queue_(deferred_queue) {
        fence_ = device_->createFence(vk::FenceCreateInfo(flags));
    }

    Fence(const Fence&) = delete;
    Fence& operator=(const Fence&) = delete;

    vk::Result wait(uint64_t timeout) {
        return device_->waitForFences({fence_}, vk::True, timeout);
    }
    void reset_fence() { device_->resetFences({fence_}); }

    vk::Fence& get_vk_fence() { return fence_; }

  private:
    vk::Fence fence_{nullptr};
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
};
} // namespace artemis
