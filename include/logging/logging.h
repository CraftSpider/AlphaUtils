#pragma once

#include <string>
#include <vector>
#include "level.h"
#include "logger.h"

namespace logging {

void __ensure_loggers();

void set_default_level(Level* level);
void add_default_handler(Handler* handler);
bool remove_default_handler(Handler* handler);
Logger* get_root_logger();
Logger* get_logger(const std::string& name, bool auto_parent = true);

}
