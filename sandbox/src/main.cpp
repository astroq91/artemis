#include "artemis/core/application.hpp"
void onUpdate(float) {}
int main() {
    Artemis::Application app;
    app.setUpdateCallback(onUpdate).run();
}
