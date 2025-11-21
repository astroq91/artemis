#include "log.hpp"
namespace artemis {
std::shared_ptr<utils::Logger> Log::logger_ = nullptr;
void Log::init() { logger_ = utils::Logger::create("ARTEMIS"); }
} // namespace artemis
