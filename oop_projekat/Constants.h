#ifndef CONSTANTS
#define CONSTANTS

namespace constants
{
	constexpr int infinity = 9999999;
	constexpr int impossible_weight = 10000;

	const std::regex re_pattern_stops(R"(^([0-9]+) (.+)$)");
	const std::regex re_pattern_lines(R"(^(\w+) \[([0-9]{2}):([0-9]{2})-([0-9]{2}):([0-9]{2})#([0-9]{1,2})\] ([0-9 ]+)$)");
	const std::regex re_pattern_pathfinding_stop_name(R"(^(\d+)_(\w+)$)");

}

#endif
