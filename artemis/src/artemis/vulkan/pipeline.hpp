#pragma once

#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan_raii.hpp>
namespace artemis {

/**
 * Specification for a pipeline.
 */
struct PipelineInfo {
    const Shader* vertex_shader;
    const Shader* fragment_shader;
    vk::Format swap_chain_image_format;
};

class Pipeline {
  public:
    Pipeline() = default;

    Pipeline(const VulkanContext& context, const PipelineInfo& info);

    const vk::raii::Pipeline& get_vk_pipeline() const { return pipeline_; }

  private:
    vk::raii::Pipeline pipeline_{nullptr};
    vk::raii::PipelineLayout layout_{nullptr};
};
} // namespace artemis
