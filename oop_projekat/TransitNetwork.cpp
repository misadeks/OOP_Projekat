#include "TransitNetwork.h"

#include <iostream>
#include <regex>

void TransitNetwork::load_data(const std::string& stops_filename, const std::string& transit_lines_filename)
{
	//move this to constants
	const std::regex regex_stops(R"(^([0-9]+) (.+)$)");
	const std::regex regex_lines(
		R"(^(\w+) \[([0-9]{2}):([0-9]{2})-([0-9]{2}):([0-9]{2})#([0-9]{1,2})\] ([0-9 ]+)$)");
	

	std::ifstream stops_input(stops_filename);
	std::ifstream lines_input(transit_lines_filename);

	//try to merge
	if(stops_input.is_open())
	{
		std::string line;
		while (std::getline(lines_input, line))
		{
			try {
				std::smatch match;
				std::regex_search(line, match, regex_stops);
				if (match.empty())
				{
					throw std::exception("GRESKA!");
				}
				this->stops[std::stoi(match[1])] = match[2];
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
	}
	else
	{
		//read error
	}

	if (lines_input.is_open())
	{
		std::string line;
		while (std::getline(lines_input, line))
		{
			try {
				std::smatch match;
				std::regex_search(line, match, regex_stops);
				if (match.empty())
				{
					throw std::exception("GRESKA!");
				}
				this->transit_lines[match[1]] = Line(match);
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
	}
	else
	{
		//file read error
	}


}
