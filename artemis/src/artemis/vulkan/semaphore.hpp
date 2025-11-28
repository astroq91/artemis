#pragma once

#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {
class Semaphore {
  public:
    Semaphore() = default;
    ~Semaphore() {
        if (semaphore_ != nullptr) {
            deferred_queue_->enqueue([dev = device_, semaphore = semaphore_]() {
                dev->destroySemaphore(semaphore);
            });
        }
    }
    Semaphore(VulkanContext* context, DeferredQueue* deferred_queue)
        : device_(context->device.get()), deferred_queue_(deferred_queue) {
        semaphore_ = device_->createSemaphore({});
    }

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

    vk::Semaphore& get_vk_semaphore() { return semaphore_; }

  private:
    vk::Semaphore semaphore_{nullptr};
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
};
} // namespace artemis
