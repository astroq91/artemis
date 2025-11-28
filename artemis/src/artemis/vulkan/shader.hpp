#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include "artemis/assets/deferred_queue.hpp"
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
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    /**
     * Creates a shader.
     * @param context The vulkan context.
     * @param file The path to the given shader file.
     * @param type All types of shaders contained in the file.
     */
    Shader(const std::string& file, const ShaderType& type,
           const VulkanContext& context, DeferredQueue* deferred_queue);

    /**
     * Get the type(s) contained in the shader.
     * @return The type(s).
     */
    ShaderType get_type() const { return type_; }

    /**
     * Get the internal shader module handle.
     * @return The handle.
     */
    const vk::ShaderModule& get_vk_shader_module() const { return module_; }

  private:
    vk::ShaderModule module_{nullptr};
    ShaderType type_;

    vk::Device* device_;
    DeferredQueue* deferred_queue_;
};
} // namespace artemis
