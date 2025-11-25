/**
 * The static logger used only with the artemis internal.
 */
#pragma once
#include "artemis/utils/logger.hpp"
#include <memory>
namespace artemis {
class Log {
  public:
    /**
     * Initializes the static logger.
     */
    static void init();

    /**
     * Gets the static logger.
     * @return The logger.
     */
    static std::shared_ptr<utils::Logger> get() {
        if (logger_ == nullptr) {
            assert(false);
        }
        return logger_;
    };

  private:
    static std::shared_ptr<utils::Logger> logger_;
};
} // namespace artemis
