#include "artemis/core/application.hpp"
#include "sandbox.hpp"
#include "artemis/utils/logger.hpp"
#include <memory>

int main() {
    artemis::Application app;
    {
        auto listener = std::make_unique<Sandbox>();
        app.set_listener(std::move(listener)).run();
    }
}
