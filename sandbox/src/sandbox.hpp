#pragma once
#include "artemis/core/application_listener.hpp"

class Sandbox : public artemis::ApplicationListener {
    void on_update(float ts) override;
};
