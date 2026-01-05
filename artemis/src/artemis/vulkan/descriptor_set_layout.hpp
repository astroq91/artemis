#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {
class DescriptorSetLayout {
  public:
    DescriptorSetLayout() = default;
    DescriptorSetLayout(
        VulkanContext* context, DeferredQueue* deferred_queue,
        const std::vector<vk::DescriptorSetLayoutBinding>& bindings);
    ~DescriptorSetLayout();

    const vk::DescriptorSetLayout& get_layout() const { return layout_; }

  private:
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
    vk::DescriptorSetLayout layout_{nullptr};
};
} // namespace artemis
