#include "sandbox.hpp"
#include "artemis/events/window_event.hpp"
#include <memory>
#include <print>

using namespace artemis;

void Sandbox::on_start() { logger_ = utils::Logger::create("SANDBOX"); }
void Sandbox::on_update(float ts) {}

void Sandbox::register_events(const std::shared_ptr<EventBus>& bus) {
    bus->subscribe<WindowEvent>(
        std::bind_front(&Sandbox::on_window_event, this));
}

void Sandbox::on_window_event(const WindowEvent& event) {
    logger_->info("Window event: {}", event.some_data);
}
