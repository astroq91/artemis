#include "descriptor_set.hpp"
#include "artemis/core/log.hpp"
#include "vulkan/vulkan.hpp"
namespace artemis {

DescriptorSet::DescriptorSet(VulkanContext* context,
                             DeferredQueue* deferred_queue,
                             const vk::DescriptorSetAllocateInfo& info)
    : device_(context->device.get()), deferred_queue_(deferred_queue),
      pool_(info.descriptorPool) {

    if (context->device->allocateDescriptorSets(&info, &set_) !=
        vk::Result::eSuccess) {
        Log::get()->error("(vk) Failed to allocate descriptor set.");
    }
}


void DescriptorSet::update(const WriteDescriptorSet&& info) {
    device_->updateDescriptorSets(vk::WriteDescriptorSet {
      set_,
      info.dst_binding,
      info.dst_array_element,
      info.descriptor_count,
      info.descriptor_type,
      info.p_image_info,
      info.p_buffer_info,
    }, {});
}

DescriptorSet::~DescriptorSet() {
    if (set_ != nullptr) {
        deferred_queue_->enqueue(
            [device = device_, set = set_, pool = pool_]() {
                device->freeDescriptorSets(pool, set);
            });
    }
}
} // namespace artemis
