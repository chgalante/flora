#pragma once

#include "pch.h"
#include "Flora/Base.h"

// Ignore warnings raised inside external headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace FloraEngine {
  class Log
  {
    public:
    static inline void Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("FloraEngine.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		sCoreLogger = std::make_shared<spdlog::logger>("FLORA", begin(logSinks), end(logSinks));
		spdlog::register_logger(sCoreLogger);
		sCoreLogger->set_level(spdlog::level::trace);
		sCoreLogger->flush_on(spdlog::level::trace);

		sClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(sClientLogger);
		sClientLogger->set_level(spdlog::level::trace);
		sClientLogger->flush_on(spdlog::level::trace);
	}

    static inline Ref<spdlog::logger>& GetCoreLogger() { return sCoreLogger;}
    static inline Ref<spdlog::logger>& GetClientLogger() { return sClientLogger;}

  private:
    static inline Ref<spdlog::logger> sCoreLogger;
    static inline Ref<spdlog::logger> sClientLogger;

  };
}

#define FE_CORE_TRACE(...) ::FloraEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define FE_CORE_INFO(...) ::FloraEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define FE_CORE_WARN(...) ::FloraEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define FE_CORE_ERROR(...) ::FloraEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define FE_CORE_CRITICAL(...) ::FloraEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define FE_TRACE(...) ::FloraEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define FE_INFO(...) ::FloraEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define FE_WARN(...) ::FloraEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define FE_ERROR(...) ::FloraEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define FE_CRITICAL(...) ::FloraEngine::Log::GetClientLogger()->critical(__VA_ARGS__)