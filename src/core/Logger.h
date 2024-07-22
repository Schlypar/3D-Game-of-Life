#ifndef LOGGER_H_
#define LOGGER_H_

#define FMT_HEADER_ONLY 1

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_color_sinks.h>

#include <memory>

namespace GoL {

class Logger {
private:
    template <typename T>
    using Ref = std::shared_ptr<T>;

private:
    static Ref<spdlog::logger> OpenGLLogger;
    static Ref<spdlog::logger> GoLLogger;

public:
    static void Init();
    static spdlog::logger* GetOpenGLLogger();
    static spdlog::logger* GetGoLLogger();
    static void SetLevel(spdlog::level::level_enum level);
};

}

#endif // LOGGER_H_

#if defined(DEBUG)
    #define OPENGL_TRACE(...) Logger::GetOpenGLLogger()->trace(__VA_ARGS__)
    #define OPENGL_INFO(...) Logger::GetOpenGLLogger()->info(__VA_ARGS__)
    #define OPENGL_WARN(...) Logger::GetOpenGLLogger()->warn(__VA_ARGS__)
    #define OPENGL_ERROR(...) Logger::GetOpenGLLogger()->error(__VA_ARGS__)
    #define OPENGL_FATAL(...) Logger::GetOpenGLLogger()->critical(__VA_ARGS__)

    #define CORE_TRACE(...) Logger::GetGoLLogger()->trace(__VA_ARGS__)
    #define CORE_INFO(...) Logger::GetGoLLogger()->info(__VA_ARGS__)
    #define CORE_WARN(...) Logger::GetGoLLogger()->warn(__VA_ARGS__)
    #define CORE_ERROR(...) Logger::GetGoLLogger()->error(__VA_ARGS__)
    #define CORE_FATAL(...) Logger::GetGoLLogger()->critical(__VA_ARGS__)
#else
    #define OPENGL_TRACE(...)
    #define OPENGL_INFO(...)
    #define OPENGL_WARN(...)
    #define OPENGL_ERROR(...)
    #define OPENGL_FATAL(...)

    #define CORE_TRACE(...)
    #define CORE_INFO(...)
    #define CORE_WARN(...)
    #define CORE_ERROR(...)
    #define CORE_FATAL(...)
#endif
