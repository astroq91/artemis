#pragma once
#include "spdlog/spdlog.h"
#include <memory>

namespace artemis {
void init_logger();
const std::shared_ptr<spdlog::logger>& get_logger();

#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(get_logger(), __VA_ARGS__)
#define LOG_DEBUG(...) SPDLOG_LOGGER_DEBUG(get_logger(), __VA_ARGS__)
#define LOG_INFO(...) SPDLOG_LOGGER_INFO(get_logger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(get_logger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(get_logger(), __VA_ARGS__)
#define LOG_CRIT(...)                                                          \
    SPDLOG_LOGGER_CRITICAL(get_logger(), __VA_ARGS__);                         \
    assert(false)
} // namespace artemis
