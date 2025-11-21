#include "application.hpp"
#include "artemis/core/log.hpp"

namespace artemis {
void Application::run() {
    init_logger();
    window_ = std::make_unique<Window>();
    while (running_) {
        listener_->on_update(1.0f);
    }
}
} // namespace artemis
