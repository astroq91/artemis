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

  private:
    vk::Fence fence_;
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
};
} // namespace artemis
