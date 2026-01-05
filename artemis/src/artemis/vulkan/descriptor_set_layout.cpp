#include "descriptor_set_layout.hpp"
#include "artemis/core/log.hpp"
#include "vulkan/vulkan.hpp"

namespace artemis {

DescriptorSetLayout::DescriptorSetLayout(
    VulkanContext* context, DeferredQueue* deferred_queue,
    const std::vector<vk::DescriptorSetLayoutBinding>& bindings)
    : device_(context->device.get()), deferred_queue_(deferred_queue) {
    vk::DescriptorSetLayoutCreateInfo info({}, bindings.size(),
                                           bindings.data());
    if (context->device->createDescriptorSetLayout(&info, nullptr, &layout_) !=
        vk::Result::eSuccess) {
        Log::get()->error("(vk) Failed to create descriptor set layout.");
    }
}

DescriptorSetLayout::~DescriptorSetLayout() {
    if (layout_ != nullptr) {
        deferred_queue_->enqueue([device = device_, layout = layout_]() {
            device->destroyDescriptorSetLayout(layout);
        });
    }
}

} // namespace artemis
