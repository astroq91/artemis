#include "descriptor_pool.hpp"
#include "artemis/core/log.hpp"
#include "vulkan/vulkan.hpp"
namespace artemis {

DescriptorPool::DescriptorPool(VulkanContext* context,
                               DeferredQueue* deferred_queue,
                               const DescriptorPoolCreateInfo& info)
    : device_(context->device.get()), deferred_queue_(deferred_queue) {

    vk::DescriptorPoolCreateInfo create_info(
        {}, info.max_sets, info.pool_sizes.size(), info.pool_sizes.data());
    if (context->device->createDescriptorPool(&create_info, nullptr, &pool_) !=
        vk::Result::eSuccess) {
        Log::get()->error("(vk) Failed to allocate descriptor pool.");
    }
}

DescriptorPool::~DescriptorPool() {
    if (pool_ != nullptr) {
        deferred_queue_->enqueue([device = device_, pool = pool_]() {
            device->destroyDescriptorPool(pool);
        });
    }
}
} // namespace artemis
