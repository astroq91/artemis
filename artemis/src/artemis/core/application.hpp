#pragma once

#include "artemis/core/application_listener.hpp"
#include <functional>
#include <memory>

#define UPDATE_CALLBACK_SIGNATURE void(float)

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
};
}; // namespace artemis
