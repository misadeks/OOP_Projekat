#include "TransitNetwork.h"

void TransitNetwork::load_data(const std::string& stops_filename, const std::string& transit_lines_filename)
{
	//move this to constants
	const std::regex re_pattern_stops(R"(^([0-9]+) (.+)$)");
	const std::regex re_pattern_lines(R"(^(\w+) \[([0-9]{2}):([0-9]{2})-([0-9]{2}):([0-9]{2})#([0-9]{1,2})\] ([0-9 ]+)$)");

	std::ifstream stops_input(stops_filename);
	std::ifstream lines_input(transit_lines_filename);

	//try to merge
	if(stops_input.is_open())
	{
		std::string line;
		while (std::getline(stops_input, line))
		{
			try {
				std::smatch match ;
				//match the regex pattern
				std::regex_search(line, match, re_pattern_stops);
				if (match.empty())
				{
					//parse error
					throw std::exception("GRESKA!");
				}
				//match[1] represents the code of the stop, match[2] represents its name
				this->stops_[std::stoi(match[1])] = match[2];
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
	}
	else
	{
		//exception
		std::cout << "file read error!\n";
	}
	
	if (lines_input.is_open())
	{
		std::string line;
		while (std::getline(lines_input, line))
		{
			try {
				std::smatch match;
				std::regex_search(line, match, re_pattern_lines);
				if (match.empty())
				{
					//parse error
					throw std::exception("GRESKA!");
				}
				//match[1] represents the line name
				this->transit_lines_[match[1]] = Line(match);
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << '\n';
			}
		}
	}
	else
	{
		std::cout << "read error!\n";
	}


}

void TransitNetwork::output_stop_info(const int stop_number)
{
	if(!stops_.contains(stop_number))
	{
		//exception stop does not exist
	}

	std::vector<std::string> found_lines;
	for (auto& [line_name, line] : transit_lines_)
	{
		std::vector<int> lookup_line = line.get_stops();
		if(std::find(lookup_line.begin(), lookup_line.end(), stop_number) != lookup_line.end())
		{
			found_lines.push_back(line_name);
		}
	}

	std::ofstream output("stajaliste_" + std::to_string(stop_number) + ".txt");
	if (output.is_open()) {
		output << stop_number << " " << stops_[stop_number] << " " << "[";
		for (std::size_t i = 0; i < found_lines.size(); i++)
		{
			output << found_lines[i];
			if (i != found_lines.size() - 1)
			{
				output << " ";
			}
		}
		output << "]";
	}
	else
	{
		//error file output
	}
	output.close();
}

void TransitNetwork::output_line_info(const std::string& line_name)
{
	//add a check if line does not exists
	if (!transit_lines_.contains(line_name))
	{
		//exception line does not exist
	}

	auto line_stops = transit_lines_[line_name].get_stops();

	std::ofstream output("linija_" + line_name + ".txt");

	if (output.is_open()) {
		output << line_name << " ";
		output << stops_[line_stops.front()] << "->" << stops_[line_stops.back()] << '\n';
		for (std::size_t i = 0; i < line_stops.size(); i++)
		{
			int stop = line_stops[i];
			output << stop << " " << stops_[stop];
			if (i != line_stops.size() - 1)
			{
				output << "\n";
			}
		}
	}
	else
	{
		//error
	}
	output.close();
}

void TransitNetwork::output_line_statistics(const std::string& line_name)
{
	if (!transit_lines_.contains(line_name))
	{
		//exception line does not exist
	}

	std::ofstream output("statistika_" + line_name + ".txt");
	if (output.is_open()) {
		const auto line_stops = transit_lines_[line_name].get_stops();
		//this set contains the found lines that intersect
		std::set<std::string> mutual_stop_lines;

		for (auto& [lookup_line_name, lookup_line	] : transit_lines_) {
			if (lookup_line_name != line_name) {
				auto lookup_line_stops = lookup_line.get_stops();
				for (auto& stop_number : line_stops) {
					if (std::find(lookup_line_stops.begin(), lookup_line_stops.end(), stop_number) != lookup_line_stops.end())
					{
						mutual_stop_lines.insert(lookup_line_name);
					}
				}
			}
		}

		output << line_name << '\n';

		std::size_t i = 0;
		for (auto& s : mutual_stop_lines)
		{
			output << s;
			if (i != mutual_stop_lines.size() - 1)
			{
				output << ' ';
			}
			i++;
		}

		output << '\n';
		output << transit_lines_[line_name].number_of_departures();
	}
	else
	{
		//error file output
	}
}

std::map<int, std::vector<std::string>> TransitNetwork::stop_timetable(const int stop_number)
{
	std::map<int, std::vector<std::string>> timetable;
	for (auto& [line_name, line] : transit_lines_)
	{
		std::vector<int> lookup_line = line.get_stops();
		auto find_result = std::find(lookup_line.begin(), lookup_line.end(), stop_number);
		if (find_result != lookup_line.end())
		{
			const int line_stop_position = std::distance(lookup_line.begin(), find_result);

			
			for(int i = 0; i < line.number_of_departures(); i++)
			{
				int calculated_time = line.get_first_departure_time() + i * line.get_interval() + line_stop_position * 3;
				if (timetable.contains(calculated_time)) {
					timetable[calculated_time].push_back(line_name);
				}
				else
				{
					timetable[calculated_time] = std::vector<std::string>({ line_name });
				}
				
			}

			
		}
	}
	return timetable;
	 auto timetable_it = timetable.lower_bound(490);
	 if(timetable_it != timetable.begin())
	 {
	 	//++timetable_it;
	 	std::cout << timetable_it->first << ": ";
	 	for (auto& line : timetable_it->second)
		{
	 		std::cout << line << " ";}
	 	std::cout << '\n';
	  }

	
	for (auto& [time, line_names] : timetable)
	{
		std::cout << time << " line: ";
		for (auto& line : line_names)
		{
			std::cout << line << " ";
		}
		std::cout << '\n';
	}
	
}

void TransitNetwork::fill_adjacency_map()
{
	for (auto& [line_name, line]: transit_lines_)
	{
		auto stops = line.get_stops();
		for (std::size_t i = 0; i < stops.size(); i++)
		{
			if (i != stops.size() - 1)
			{
				adjacency_map_[std::to_string(stops[i]) + "_" + line_name].insert(std::pair<std::string, int>{std::to_string(stops[i + 1]) + "_" + line_name, 3});
				adjacency_map_[std::to_string(stops[i]) + "_" + line_name].insert(std::pair<std::string, int>{std::to_string(stops[i]), 0}); // change to smth for the third strategy
			}
			else
			{
				//adjacency_map_[std::to_string(stops[i]) + "_" + line_name] = std::set{ std::pair<std::string, int>{} };
				adjacency_map_[std::to_string(stops[i]) + "_" + line_name].insert(std::pair<std::string, int>{std::to_string(stops[i]), 0});
			}
		}
	}
	for (auto& [stop, stop_name] : stops_)
	{
		for (auto& found_line : find_stop_lines(stop))
		{
			adjacency_map_[std::to_string(stop)].insert(std::pair<std::string, int>{std::to_string(stop) + "_" + found_line, 0});
		}
		
	}

	/*
	for (auto& [stop_id, adjacent] : adjacency_map_)
	{
		std::cout << stop_id << " : ";
		for (auto& [line_name, distance] : adjacent)
		{
			std::cout << line_name <<":"  << distance << " ";
		}
		std::cout << '\n';
	}
	*/
}

std::vector<std::string> TransitNetwork::find_stop_lines(const int stop_number)
{
	std::vector<std::string> found_lines;
	for (auto& [line_name, line] : transit_lines_)
	{
		std::vector<int> lookup_line = line.get_stops();
		if (std::find(lookup_line.begin(), lookup_line.end(), stop_number) != lookup_line.end())
		{
			found_lines.push_back(line_name);
		}
	}
	return found_lines;
}

std::map<std::string, int> TransitNetwork::find_first_departure_after(const int stop_number,
	const int time)
{
	std::map<std::string, int> lines_first_departure_after;

	auto timetable = stop_timetable(stop_number);
	auto timetable_it = timetable.lower_bound(time);
	
	while(timetable_it != timetable.end())
	{
		for(auto& line: timetable_it->second)
		{
			if (!lines_first_departure_after.contains(line))
			{
				lines_first_departure_after[line] = timetable_it->first;
			}
		}
		
		++timetable_it;
	}

	for(auto& [line_name, arrival_time] : lines_first_departure_after)
	{
		std::cout << line_name << ":" << arrival_time << '\n';
	}

	return lines_first_departure_after;
}
