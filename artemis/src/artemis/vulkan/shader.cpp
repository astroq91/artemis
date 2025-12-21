#include "shader.hpp"
#include "artemis/utils/io.hpp"
#include "artemis/vulkan/vulkan_context.hpp"

namespace artemis {
Shader::~Shader() {
    if (module_ != nullptr) {
        deferred_queue_->enqueue([dev = device_, module = module_]() {
            dev->destroyShaderModule(module);
        });
    }
}

Shader::Shader(const std::string& file, const ShaderType& type,
               VulkanContext* context, DeferredQueue* deferred_queue)
    : type_(type), device_(context->device.get()),
      deferred_queue_(deferred_queue) {

    auto src = utils::IO::read_file(file);
    vk::ShaderModuleCreateInfo create_info(
        {}, src.size() * sizeof(char),
        reinterpret_cast<const uint32_t*>(src.data()));
    module_ = context->device->createShaderModule(create_info);
}
} // namespace artemis
