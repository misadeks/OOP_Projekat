#ifndef TRANSIT_NETWORK
#define TRANSIT_NETWORK

#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "Line.h"
#include "Constants.h"
#include "Exceptions.h"

class TransitNetwork
{
public:
	void load_stops_data(const std::string& stops_filename);
	void load_transit_lines_data(const std::string& transit_lines_filename);

	void output_stop_info(const int stop_number);
	void output_line_info(const std::string& line_name);
	void output_line_statistics(const std::string& line_name);

	void find_path(const constants::strategy strategy, const int departure_stop_number, const int destination_stop_number, const std::string& departure_time_string);
private:
	std::map<std::string, Line> transit_lines_;
	std::map<int, std::string> stops_;

	std::map<std::string, std::map<std::string, int>> graph_adjacency_map_;
	std::map<std::string, std::tuple<int, bool, std::string>> graph_traversal_map_;

	std::vector<std::string> find_stop_lines(const int stop_number);

	std::string find_nearest_stop();
	void fill_adjacency_map(const constants::strategy strategy);
	std::map<int, std::vector<std::string>> stop_timetable(const int stop_number);
	std::map<std::string, int> find_first_departure_after(const int stop_number, const int time);

	void output_path(const std::string& filename, const std::vector<std::string>& path) const;
	int number_of_transfers(const std::vector<std::string>& path) const;
};

#endif

