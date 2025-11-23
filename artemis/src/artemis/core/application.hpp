#pragma once

#include "artemis/core/application_listener.hpp"
#include "artemis/core/window.hpp"
#include "artemis/events/event_bus.hpp"
#include "artemis/vulkan/vulkan_context.hpp"
#include <memory>

namespace artemis {
class Application {
  public:
    Application() = default;
    /**
     * Run the application.
     */
    void run();
    /**
     * Sets the main update function callback.
     */
    Application& set_listener(std::unique_ptr<ApplicationListener>&& listener) {
        listener_ = std::move(listener);
        return *this;
    }

  private:
    void listener_init();

  private:
    bool running_ = true;
    std::unique_ptr<ApplicationListener> listener_;
    std::unique_ptr<Window> window_;
    std::shared_ptr<EventBus> event_bus_;
    std::shared_ptr<VulkanContext> vulkan_context_;
};
}; // namespace artemis
