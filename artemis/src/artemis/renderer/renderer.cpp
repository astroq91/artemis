#include "renderer.hpp"
#include "artemis/core/log.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include <memory>
namespace artemis {

std::unique_ptr<Pipeline> mesh_pipeline;
std::unique_ptr<Pipeline> ui_pipeline;

Renderer::Renderer(VulkanContext* context, DeferredQueue* deferred_queue,
                   Window* window, uint32_t max_frames_in_flight)
    : context_(context), deferred_queue_(deferred_queue), window_(window),
      max_fif_(max_frames_in_flight) {
    init();
}
void Renderer::begin_frame() {
    while (sync_objects_[frame_idx_]->fence.wait(UINT64_MAX) ==
           vk::Result::eTimeout)
        ;
    sync_objects_[frame_idx_]->fence.reset_fence();

    command_buffers_[frame_idx_]->begin();
}
void Renderer::end_frame() {
    command_buffers_[frame_idx_]->end();
    frame_idx_ = (frame_idx_ + 1) % max_fif_;
}

void Renderer::init() {
    swap_chain_ = std::make_unique<SwapChain>(context_, window_);
    command_buffers_.resize(max_fif_);
    sync_objects_.resize(max_fif_);
    for (size_t i = 0; i < max_fif_; i++) {
        command_buffers_[i] = std::make_unique<CommandBuffer>(*context_);
        sync_objects_[i] = std::make_unique<SyncObject>(
            context_, deferred_queue_, vk::FenceCreateFlagBits::eSignaled);
    }
    Log::get()->debug("Renderer initialized.");
}
} // namespace artemis
