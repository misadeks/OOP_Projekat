#ifndef TRANSIT_NETWORK
#define TRANSIT_NETWORK

#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <regex>
#include <set>
#include <string>
#include <vector>


#include "Line.h"

class TransitNetwork
{
public:
	void load_data(const std::string& stops_filename, const std::string& transit_lines_filename);
	void output_stop_info(const int stop_number);
	void output_line_info(const std::string& line_name);
	void output_line_statistics(const std::string& line_name);

	void stop_timetable(const int stop_number);

	void fill_adjacency_map();
private:
	std::map<std::string, Line> transit_lines_;
	std::map<int, std::string> stops_;

	std::map<std::string, std::set<std::string>> adjacency_map_;

	std::vector<std::string> find_stop_lines(const int stop_number);

};
#endif

