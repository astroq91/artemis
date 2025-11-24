#include "shader.hpp"
#include "artemis/utils/io.hpp"
#include "artemis/vulkan/vulkan_context.hpp"

namespace artemis {
Shader::Shader(const VulkanContext& context, const std::string& file,
               const ShaderType& type)
    : type_(type) {
    auto src = utils::IO::read_file(file);
    vk::ShaderModuleCreateInfo create_info(
        {}, src.size() * sizeof(char),
        reinterpret_cast<const uint32_t*>(src.data()));
    module_ = vk::raii::ShaderModule(*context.device, create_info);
}
} // namespace artemis
