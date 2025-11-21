#include "logger.hpp"
#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <format>
#include <memory>

namespace artemis::utils {
void Logger::init(const std::string& name) {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern(std::format("[{} %H:%M:%S] [%^%l%$] %v", name));
    m_logger = std::make_unique<spdlog::logger>(
        "core", spdlog::sinks_init_list{console_sink});
    m_logger->set_level(spdlog::level::trace);
}

} // namespace artemis::utils
