//
// Created by lorenzo on 12/3/17.
//

#include "options.h"

bool lns::ignore_unresolved = false;
bool lns::silent_full = false;
bool lns::silent_execution = false;
bool lns::permissive = false;
bool lns::time_count = false;
bool lns::parse_only = false;
bool lns::prompt = false;
bool lns::debugger_option = false;
bool lns::no_format = false;
bool lns::update_option = false;
bool lns::no_std_option = false;
bool lns::exec_from_command_line = false;
std::string lns::command_line_source = std::string();
std::vector<std::string> lns::command_line_use = std::vector<std::string>();