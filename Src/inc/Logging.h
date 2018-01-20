#pragma once

#define SPDLOG_TRACE_ON

#include <memory>
#include <spdlog/spdlog.h>


extern std::shared_ptr<spdlog::logger> Logger;
