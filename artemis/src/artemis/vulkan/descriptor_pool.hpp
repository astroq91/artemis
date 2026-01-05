#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {
struct DescriptorPoolCreateInfo {
    size_t max_sets;
    std::vector<vk::DescriptorPoolSize> pool_sizes;
};
class DescriptorPool {
  public:
    DescriptorPool() = default;
    DescriptorPool(VulkanContext* context, DeferredQueue* deferred_queue,
                   const DescriptorPoolCreateInfo& info);
    ~DescriptorPool();

    vk::DescriptorPool get_pool() const { return pool_; }

  private:
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
    vk::DescriptorPool pool_;
};
} // namespace artemis
