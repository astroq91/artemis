#include "pipeline.hpp"
#include "artemis/assets/deferred_queue.hpp"
namespace artemis {

Pipeline::~Pipeline() {
    if (pipeline_ != nullptr) {
        deferred_queue_->enqueue(
            [dev = device_, layout = layout_, pipeline = pipeline_]() {
                dev->destroyPipelineLayout(layout);
                dev->destroyPipeline(pipeline);
            });
    }
}
Pipeline::Pipeline(const PipelineCreateInfo& info, const VulkanContext& context,
                   DeferredQueue* deferred_queue)
    : device_(context.device.get()), deferred_queue_(deferred_queue) {
    /* SHADERS */
    vk::PipelineShaderStageCreateInfo vert_shader_stage_info(
        {}, vk::ShaderStageFlagBits::eVertex,
        info.vertex_shader->get_vk_shader_module(), "vert_main");
    vk::PipelineShaderStageCreateInfo frag_shader_stage_info(
        {}, vk::ShaderStageFlagBits::eFragment,
        info.fragment_shader->get_vk_shader_module(), "frag_main");

    vk::PipelineShaderStageCreateInfo shader_stages[] = {
        vert_shader_stage_info, frag_shader_stage_info};

    /* DYNAMIC STATE */
    std::vector dynamic_states = {
        vk::DynamicState::eViewport,
        vk::DynamicState::eScissor,
    };

    vk::PipelineDynamicStateCreateInfo dynamic_state({}, dynamic_states.size(),
                                                     dynamic_states.data());

    /* VERTEX INPUT */
    vk::PipelineVertexInputStateCreateInfo vertex_input_info;

    /* INPUT ASSEMBLY */
    vk::PipelineInputAssemblyStateCreateInfo input_assembly(
        {}, vk::PrimitiveTopology::eTriangleList);

    /* VIEWPORT & SCISSOR */
    vk::PipelineViewportStateCreateInfo viewport_info({}, 1, {}, 1);

    /* RASTERIZER */
    vk::PipelineRasterizationStateCreateInfo rasterizer(
        {}, vk::False, vk::False, vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eBack, vk::FrontFace::eClockwise, vk::False);

    /* MULTISAMPLING */
    vk::PipelineMultisampleStateCreateInfo multisampling(
        {}, vk::SampleCountFlagBits::e1, vk::False);

    /* DEPTH & STENCIL */
    // todo...

    /* COLOR BLENDING */
    vk::PipelineColorBlendAttachmentState color_blend_attachment;
    color_blend_attachment.colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    color_blend_attachment.blendEnable = vk::False;

    vk::PipelineColorBlendStateCreateInfo color_blending(
        {}, vk::False, vk::LogicOp::eCopy, 1, &color_blend_attachment);

    /* DYNAMIC RENDERING */
    vk::PipelineRenderingCreateInfo pipeline_rendering_info;
    pipeline_rendering_info.colorAttachmentCount = 1;
    pipeline_rendering_info.pColorAttachmentFormats =
        &info.swap_chain_image_format;

    /* PIPELINE LAYOUT */
    vk::PipelineLayoutCreateInfo pipeline_layout_info({}, 0, nullptr, 0,
                                                      nullptr);
    layout_ = context.device->createPipelineLayout(pipeline_layout_info);

    /* PIPELINE CREATION */
    vk::GraphicsPipelineCreateInfo pipeline_info;
    pipeline_info.pNext = &pipeline_rendering_info;
    pipeline_info.stageCount = 2;
    pipeline_info.pStages = shader_stages;
    pipeline_info.pVertexInputState = &vertex_input_info;
    pipeline_info.pInputAssemblyState = &input_assembly;
    pipeline_info.pViewportState = &viewport_info;
    pipeline_info.pRasterizationState = &rasterizer;
    pipeline_info.pMultisampleState = &multisampling;
    pipeline_info.pColorBlendState = &color_blending;
    pipeline_info.pDynamicState = &dynamic_state;
    pipeline_info.layout = layout_;
    pipeline_info.renderPass = nullptr;

    pipeline_ =
        context.device->createGraphicsPipeline(nullptr, pipeline_info).value;
}

} // namespace artemis
