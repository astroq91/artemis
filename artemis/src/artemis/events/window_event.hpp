#pragma once

#include "artemis/events/event.hpp"
#include <string>
namespace artemis {

struct WindowEvent : public Event {
    std::string some_data;
};

} // namespace artemis
