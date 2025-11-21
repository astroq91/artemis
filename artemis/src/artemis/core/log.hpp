#pragma once
#include "artemis/utils/logger.hpp"
#include <memory>
namespace artemis {
class Log {
  public:
    static void init();
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
