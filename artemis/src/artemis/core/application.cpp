#include "application.hpp"

namespace artemis {
void Application::run() {
    while (running_) {
        listener_->on_update(1.0f);
    }
}
} // namespace artemis
