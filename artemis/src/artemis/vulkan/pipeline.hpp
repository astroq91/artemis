#pragma once

#include "artemis/assets/resource_library.hpp"
#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
namespace artemis {

struct PipelineInfo {
    Shader* vertex_shader;
    Shader* fragment_shader;
};

class Pipeline {
  public:
    Pipeline() = default;

    Pipeline(const VulkanContext& context, const PipelineInfo& info);
};
} // namespace artemis
