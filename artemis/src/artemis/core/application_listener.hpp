#pragma once

#include "artemis/events/event_bus.hpp"
#include <memory>
namespace artemis {
class ApplicationListener {
  public:
    virtual ~ApplicationListener() = default;

    virtual void on_start() = 0;
    /**
     * A method called once every update tick.
     *
     * @param ts The timestep.
     */
    virtual void on_update(float ts) = 0;

    /**
     * Called once during startup, used to
     * subscribe to events.
     *
     * @param bus The system used to subscribe
     *            to event types.
     */
    virtual void register_events(const std::shared_ptr<EventBus>& bus) = 0;
};
} // namespace artemis
