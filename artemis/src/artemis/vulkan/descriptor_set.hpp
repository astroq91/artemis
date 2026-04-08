#pragma once
#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include "vulkan/vulkan.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {

struct WriteDescriptorSet {
  uint32_t dst_binding;
  uint32_t descriptor_count;
  uint32_t dst_array_element;
  vk::DescriptorType descriptor_type;
  vk::DescriptorImageInfo*  p_image_info = nullptr;
  vk::DescriptorBufferInfo* p_buffer_info = nullptr;
};

class DescriptorSet {
  public:
    DescriptorSet() = default;
    DescriptorSet(VulkanContext* context, DeferredQueue* deferred_queue,
                  const vk::DescriptorSetAllocateInfo& info);
    ~DescriptorSet();

    void update(const WriteDescriptorSet&& info);

    vk::DescriptorSet get_set() const { return set_; }

  private:
    vk::Device* device_;
    DeferredQueue* deferred_queue_;
    vk::DescriptorPool pool_;

    vk::DescriptorSet set_{nullptr};
};
} // namespace artemis
