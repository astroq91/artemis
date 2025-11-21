#pragma once

#include <functional>

#define UPDATE_CALLBACK_SIGNATURE void(float)

namespace Artemis {
class Application {
  public:
    Application() = default;
    /**
     * Sets the main update function callback.
     */
    Application& setUpdateCallback(
        const std::function<UPDATE_CALLBACK_SIGNATURE>& callback) {
        m_update_callback = callback;
        return *this;
    }

  private:
    std::function<void(float)> m_update_callback;
};
}; // namespace Artemis
