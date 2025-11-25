#pragma once

#include "artemis/core/application_context.hpp"
namespace artemis {
class ApplicationListener {
  public:
    virtual ~ApplicationListener() = default;

    virtual void on_start(ApplicationContext* context) = 0;
    /**
     * A method called once every update tick.
     *
     * @param ts The timestep.
     */
    virtual void on_update(float ts) = 0;
};
} // namespace artemis
