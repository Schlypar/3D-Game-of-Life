#include "Logger.h"

namespace GoL {

std::shared_ptr<spdlog::logger> Logger::OpenGLLogger;
std::shared_ptr<spdlog::logger> Logger::GoLLogger;

void Logger::Init() {
    OpenGLLogger = spdlog::stdout_color_mt("OpenGL");
    GoLLogger = spdlog::stdout_color_mt("Core");

    OpenGLLogger->set_pattern("%^[%T][thread:%t][%l]%$ (%n): %v (%o ms)");
    GoLLogger->set_pattern("%^[%T][thread:%t][%l]%$ (%n): %v (%o ms)");

    OpenGLLogger->set_level(spdlog::level::trace);
    GoLLogger->set_level(spdlog::level::trace);
}

spdlog::logger* Logger::GetOpenGLLogger() {
    return OpenGLLogger.get();
}

spdlog::logger* Logger::GetGoLLogger() {
    return GoLLogger.get();
}

void Logger::SetLevel(spdlog::level::level_enum level) {
    OpenGLLogger->set_level(level);
    GoLLogger->set_level(level);
}

}
