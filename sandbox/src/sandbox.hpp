#pragma once
#include "artemis/core/application_listener.hpp"
#include "artemis/events/event_bus.hpp"

class Sandbox : public artemis::ApplicationListener {
    void on_update(float ts) override;
    void register_events(const artemis::EventBus& bus) override;
};
