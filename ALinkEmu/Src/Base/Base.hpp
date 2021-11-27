//
// Created by Norbert Bielak on 18.11.2021.
//
#pragma once

#include <csignal>

#include "Logger.hpp"
#define EMU_DEBUG_BREAK() raise(SIGABRT)

#define EMU_ASSERT(x, ...)                                       \
  if (!(x)) {                                                    \
    EMU_LOG_CRITICAL("ALinkEmu debug assertion failed!"); \
    EMU_LOG_CRITICAL(__VA_ARGS__);                        \
    EMU_DEBUG_BREAK();                                            \
  }

namespace ALinkEmu {}
