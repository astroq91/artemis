#pragma once

#include "artemis/assets/deferred_queue.hpp"
#include "artemis/vulkan/shader.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include "artemis/vulkan/vertex_buffer_description.hpp"
#include <vulkan/vulkan.hpp>
namespace artemis {

/**
 * Specification for a pipeline.
 */
struct PipelineCreateInfo {
    const Shader* vertex_shader;
    const Shader* fragment_shader;
    vk::Format swap_chain_image_format;
    std::vector<VertexBufferDescription> vertex_buffer_descs;
};

class Pipeline {
  public:
    Pipeline() = default;
    ~Pipeline();

    Pipeline(VulkanContext* context, DeferredQueue* deferred_queue,
             const PipelineCreateInfo& info);

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
