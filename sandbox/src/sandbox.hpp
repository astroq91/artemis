#pragma once
#include "artemis/core/application_context.hpp"
#include "artemis/core/application_listener.hpp"
#include "artemis/events/event_bus.hpp"
#include "artemis/events/window_event.hpp"
#include "artemis/utils/logger.hpp"
#include "artemis/vulkan/command_buffer.hpp"
#include "artemis/vulkan/pipeline.hpp"
#include "artemis/vulkan/swap_chain.hpp"
#include <memory>

class Sandbox : public artemis::ApplicationListener {
    void on_start(artemis::ApplicationContext* context) override;
    void on_update(float ts) override;

  private:
    void on_window_event(const artemis::WindowEvent& event);

  private:
    std::shared_ptr<artemis::utils::Logger> logger_;
    artemis::ApplicationContext* app_context_;

    artemis::CommandBuffer command_buffer_;
    artemis::SwapChain swap_chain_;
    artemis::Pipeline pipeline_;
    artemis::Shader shader_;
    uint32_t image_index_ = 0;
};
