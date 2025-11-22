#pragma once
#include "artemis/core/application_listener.hpp"
#include "artemis/events/event_bus.hpp"
#include "artemis/events/window_event.hpp"
#include "artemis/utils/logger.hpp"
#include <memory>

class Sandbox : public artemis::ApplicationListener {
    void on_start() override;
    void on_update(float ts) override;
    void
    register_events(const std::shared_ptr<artemis::EventBus>& bus) override;

  private:
    void on_window_event(const artemis::WindowEvent& event);

  private:
    std::shared_ptr<artemis::utils::Logger> logger_;
};
