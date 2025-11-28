#pragma once

#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <vulkan/vulkan.hpp>
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
    ~Pipeline();

    Pipeline(const PipelineInfo& info, const VulkanContext& context,
             DeferredQueue* deferred_queue);

    Pipeline(const Pipeline&) = delete;
    Pipeline& operator=(const Pipeline&) = delete;

    const vk::Pipeline& get_vk_pipeline() const { return pipeline_; }

  private:
    vk::Pipeline pipeline_{nullptr};
    vk::PipelineLayout layout_{nullptr};

    vk::Device* device_;
    DeferredQueue* deferred_queue_;
};
} // namespace artemis
