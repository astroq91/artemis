#pragma once

#include <vulkan/vulkan_raii.hpp>
#include "artemis/utils/enum_flags.hpp"
#include "artemis/vulkan/vulkan_context.hpp"

namespace artemis {

/**
 * The shader type(s).
 */
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

    /**
     * Creates a shader.
     * @param context The vulkan context.
     * @param file The path to the given shader file.
     * @param type All types of shaders contained in the file.
     */
    Shader(const VulkanContext& context, const std::string& file,
           const ShaderType& type);

    /**
     * Get the type(s) contained in the shader.
     * @return The type(s).
     */
    ShaderType get_type() const { return type_; }

    /**
     * Get the internal shader module handle.
     * @return The handle.
     */
    const vk::raii::ShaderModule& get_module() const { return module_; }

  private:
    vk::raii::ShaderModule module_{nullptr};
    ShaderType type_;
};
} // namespace artemis
