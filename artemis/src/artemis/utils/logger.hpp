#pragma once
#include "spdlog/spdlog.h"
#include <format>
#include <memory>
#include <utility>

namespace artemis::utils {
class Logger {
  public:
    static auto create(const std::string& name) -> std::shared_ptr<Logger> {
        auto obj = std::make_shared<Logger>();
        obj->init(name);
        return obj;
    }
    void trace(const std::string& msg) { m_logger->trace(msg); }
    template <typename... Args>
    void trace(const fmt::format_string<Args...>& fmt, Args&&... args) {
        m_logger->trace(fmt, std::forward<Args>(args)...);
    }
    void debug(const std::string& msg) { m_logger->debug(msg); }
    template <typename... Args>
    void debug(const fmt::format_string<Args...>& fmt, Args&&... args) {
        m_logger->debug(fmt, std::forward<Args>(args)...);
    }
    void info(const std::string& msg) { m_logger->info(msg); }
    template <typename... Args>
    void info(const fmt::format_string<Args...>& fmt, Args&&... args) {
        m_logger->info(fmt, std::forward<Args>(args)...);
    }
    void error(const std::string& msg) { m_logger->error(msg); }
    template <typename... Args>
    void error(const fmt::format_string<Args...>& fmt, Args&&... args) {
        m_logger->error(fmt, std::forward<Args>(args)...);
    }
    void critical(const std::string& msg) {
        m_logger->critical(msg);
        assert(false);
    }
    template <typename... Args>
    void critical(const fmt::format_string<Args...>& fmt, Args&&... args) {
        m_logger->critical(fmt, std::forward<Args>(args)...);
        assert(false);
    }

  private:
    void init(const std::string& name);

  private:
    std::unique_ptr<spdlog::logger> m_logger;
};
} // namespace artemis::utils
