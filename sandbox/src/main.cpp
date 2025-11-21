#include "artemis/core/application.hpp"
#include "sandbox.hpp"
#include "log.hpp"
#include <memory>

int main() {
    init_logger();
    artemis::Application app;
    auto listener = std::make_unique<Sandbox>();
    app.set_listener(std::move(listener)).run();
}
