#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {
class DescriptorSet {
  public:
    DescriptorSet() = default;
    DescriptorSet(VulkanContext* context, DeferredQueue* deferred_queue,
                  const vk::DescriptorSetAllocateInfo& info);
    ~DescriptorSet();

    vk::DescriptorSet get_set() const { return set_; }

  private:
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
    vk::DescriptorPool pool_;

    vk::DescriptorSet set_{nullptr};
};
} // namespace artemis
