#include "pipeline.hpp"
#include <stdexcept>
#include <vulkan/vulkan_structs.hpp>

namespace artemis {
Pipeline::Pipeline(const VulkanContext& context, const PipelineInfo& info) {
    vk::PipelineShaderStageCreateInfo vert_shader_stage_info(
        {}, vk::ShaderStageFlagBits::eVertex, info.vertex_shader->get_module(),
        "vert_main");
    vk::PipelineShaderStageCreateInfo frag_shader_stage_info(
        {}, vk::ShaderStageFlagBits::eFragment,
        info.fragment_shader->get_module(), "frag_main");

    vk::PipelineShaderStageCreateInfo shader_stages[] = {
        vert_shader_stage_info, frag_shader_stage_info};
}
} // namespace artemis
