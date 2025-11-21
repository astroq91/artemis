#include "application.hpp"
#include "artemis/core/log.hpp"

namespace artemis {

void Application::run() {
    Log::init();
    window_ = std::make_unique<Window>();
    while (running_) {
        if (window_->should_close()) {
            running_ = false;
        }
        listener_->on_update(1.0f);
    }
}

} // namespace artemis
