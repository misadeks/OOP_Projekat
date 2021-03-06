#ifndef CONSTANTS
#define CONSTANTS

#include <regex>

namespace constants
{
	const int infinity = 9999999;
	const int impossible_weight = 10000;
	const int inter_stop_distance = 3;

	const std::regex re_pattern_stops(R"(^([0-9]+) (.+)$)");
	const std::regex re_pattern_lines(R"(^(\w+) \[([0-9]{2}):([0-9]{2})-([0-9]{2}):([0-9]{2})#([0-9]{1,2})\] ([0-9 ]+)$)");
	const std::regex re_pattern_pathfinding_stop_name(R"(^(\d+)_(\w+)$)");
	const std::regex re_pattern_time(R"(^([0-9]{2}):([0-9]{2})$)");

	enum class strategy
	{
		FASTEST, LEAST_TRANSFERS
	};

	const std::string console_red_text = "\033[31;1m";
	const std::string console_green_text = "\033[32m";
	const std::string console_magenta_text = "\033[35m";
	const std::string console_yellow_text = "\033[33;1m";
	const std::string console_cyan_text = "\033[38;5;123m";
	const std::string underline_console = "\033[4m";
	const std::string no_underline_console = "\033[24m";
	const std::string console_default = "\033[0m";

	const std::string loading_text = R"(                      _     _    _                                
                     | |   | |  (_)                               
   __ _ _ __ __ _  __| |___| | ___   _ __  _ __ _____   _____ ____
  / _` | '__/ _` |/ _` / __| |/ | | | '_ \| '__/ _ \ \ / / _ |_  /
 | (_| | | | (_| | (_| \__ |   <| | | |_) | | |  __/\ V | (_) / / 
  \__, |_|  \__,_|\__,_|___|_|\_|_| | .__/|_|  \___| \_/ \___/___|
   __/ |                            | |                           
  |___/                             |_|   Mihajlo Krunić 2020/0366)";

	const std::string train = R"(___________   _______________________________________^__
 ___   ___ |||  ___   ___   ___    ___ ___  |   __  ,----\
|   | |   |||| |   | |   | |   |  |   |   | |  |  | |_____\
|___| |___|||| |___| |___| |___|  | O | O | |  |  |        \
           |||                    |___|___| |  |__|         )
___________|||______________________________|______________/
           |||                                        /--------
-----------'''---------------------------------------')";
}

#endif
