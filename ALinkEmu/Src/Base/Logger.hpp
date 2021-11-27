//
// Created by Norbert Bielak on 17.11.2021.
//

#pragma  once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>


class Logger {
 public:
  static inline void Init() {
    spdlog::set_pattern("%^[%d-%m-%Y %H:%M:%S] [%n-%l] [Thread - %t] %v%$");
    emuLogger = spdlog::stdout_color_mt("EMU");
    emuLogger->set_level(spdlog::level::trace);
    clientLogger = spdlog::stdout_color_mt("ClientApp");
    clientLogger->set_level(spdlog::level::trace);
  }

  static inline std::shared_ptr<spdlog::logger>& GetEmuLogger() {
    return emuLogger;
  }

  static inline std::shared_ptr<spdlog::logger>& GetClientLogger() {
    return clientLogger;
  }

 private:
  static inline std::shared_ptr<spdlog::logger> emuLogger;
  static inline std::shared_ptr<spdlog::logger> clientLogger;
};


// Core log macros
#define EMU_LOG_TRACE(...)    Logger::GetEmuLogger()->trace(__VA_ARGS__)
#define EMU_LOG_INFO(...)     Logger::GetEmuLogger()->info(__VA_ARGS__)
#define EMU_LOG_WARN(...)     Logger::GetEmuLogger()->warn(__VA_ARGS__)
#define EMU_LOG_ERROR(...)    Logger::GetEmuLogger()->error(__VA_ARGS__)
#define EMU_LOG_CRITICAL(...) Logger::GetEmuLogger()->critical(__VA_ARGS__)

// Client log macros
#define EMU_CLIENT_LOG_TRACE(...)           Logger::GetClientLogger()->trace(__VA_ARGS__)
#define EMU_CLIENT_LOG_INFO(...)            Logger::GetClientLogger()->info(__VA_ARGS__)
#define EMU_CLIENT_LOG_WARN(...)            Logger::GetClientLogger()->warn(__VA_ARGS__)
#define EMU_CLIENT_LOG_ERROR(...)           Logger::GetClientLogger()->error(__VA_ARGS__)
#define EMU_CLIENT_LOG_CRITICAL(...)        Logger::GetClientLogger()->critical(__VA_ARGS__)



