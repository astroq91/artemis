#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "artemis/utils/enum_flags.hpp"
#include "artemis/vulkan/vulkan_context.hpp"

namespace artemis {

enum class ShaderType : uint32_t {
    None = 0,
    Vertex = 1 << 0,
    Fragment = 1 << 1,
    // etc.
};
template <> inline constexpr bool is_flag_enum<ShaderType> = true;

class Shader {
  public:
    Shader() = default;

    Shader(const VulkanContext& context, const std::string& file,
           const ShaderType& type);

    ShaderType get_type() const { return type_; }
    const vk::raii::ShaderModule& get_module() const { return module_; }

  private:
    vk::raii::ShaderModule module_{nullptr};
    ShaderType type_;
};
} // namespace artemis
