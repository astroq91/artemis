#pragma once

namespace artemis {
class ApplicationListener {
  public:
    virtual ~ApplicationListener() = default;
    /**
     * A method called once every update tick.
     * @param ts The timestep.
     */
    virtual void on_update(float ts) = 0;
};
} // namespace artemis
