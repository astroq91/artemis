#pragma once

#include "artemis/core/application_listener.hpp"
#include "artemis/core/window.hpp"
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
    bool running_ = true;
    std::unique_ptr<ApplicationListener> listener_;
    std::unique_ptr<Window> window_;
};
}; // namespace artemis
