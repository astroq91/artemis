#include "log.hpp"
#include "spdlog/common.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"
#include <memory>

namespace artemis {
static std::shared_ptr<spdlog::logger> s_logger;

void init_logger() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_pattern("[ARTEMIS %H:%M:%S] [%^%l%$] %v");
    s_logger = std::make_shared<spdlog::logger>(
        "core", spdlog::sinks_init_list{console_sink});
    s_logger->set_level(spdlog::level::trace);
    spdlog::register_logger(s_logger);
}

const std::shared_ptr<spdlog::logger>& get_logger() { return s_logger; }

} // namespace artemis
