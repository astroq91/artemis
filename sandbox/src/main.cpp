#include "artemis/core/application.hpp"
#include "sandbox.hpp"
#include <memory>

using namespace artemis;
int main() {
    Application app;
    auto listener = std::make_unique<Sandbox>();
    app.set_listener(std::move(listener)).run();
}
