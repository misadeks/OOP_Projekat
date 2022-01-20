#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include "TransitNetwork.h"

std::vector<std::string> split_line(const std::string& s)
{
	std::string temp;
	std::stringstream ss(s);
	std::vector<std::string> line;

	while (getline(ss, temp, ' ')) {
		line.push_back(temp);
	}
	return line;
}

int main()
{
	/*
	std::regex re_string = std::regex(
		R"(^(\w+) \[([0-9]{2}):([0-9]{2})-([0-9]{2}):([0-9]{2})#([0-9]{1,2})\] ([0-9 ]+)$)");
	
	std::string test = "12 [05:30-23:30#15] 154 578 123 422 3112 99 12 1221 1212";
	std::smatch match;
	std::regex_search(test, match, re_string);
	
	//std::cout << match[1] << '\n';

	std::map<std::string, std::vector<std::string>> transit_lines_;

	transit_lines_[match[1]] = split_line(match[7]);
	
	for (auto e: transit_lines_)
	{
		std::cout << e.first << '\n';
		for (auto a : e.second)
		{
			std::cout << a << '\n';
		}
	}
	
	
	
	std::ifstream input("linije.txt");

	if (input.is_open()) {

	}
	*/
	TransitNetwork a;

	a.load_data("stajalista.txt", "linije.txt");

	/*
	for (auto& e: a.transit_lines_)
	{
		std::cout << e.first << '\n';
		for(auto stop: e.second.stops_)
		{
			std::cout << stop << '\n';
		}
	}
	
	for (auto& e: a.stops_)
	{
		std::cout << e.first << "->" << e.second << '\n';
	}
	*/
	/*
	a.output_stop_info(422);
	a.output_line_info("12");
	*/
	//a.output_line_statistics("12");
	//a.stop_timetable(1024);
	a.fill_adjacency_map();

	return 0;
}


