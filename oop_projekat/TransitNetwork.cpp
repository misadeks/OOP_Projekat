#include "TransitNetwork.h"


void TransitNetwork::load_stops_data(const std::string& stops_filename)
{
	std::ifstream stops_input(stops_filename);

	if (stops_input.is_open())
	{
		int line_count = 1;
		std::string line;
		while (std::getline(stops_input, line))
		{
			std::smatch match;
			//match the regex pattern
			std::regex_search(line, match, constants::re_pattern_stops);
			if (match.empty())
			{
				throw ParseError(line_count);
			}
			//match[1] represents the code of the stop, match[2] represents its name
			this->stops_[std::stoi(match[1])] = match[2];
			line_count++;
		}
	}
	else
	{
		throw FileReadError();
	}
}

void TransitNetwork::load_transit_lines_data(const std::string& transit_lines_filename)
{
	std::ifstream lines_input(transit_lines_filename);

	if (lines_input.is_open())
	{
		int line_count = 1;
		std::string line;
		while (std::getline(lines_input, line))
		{
			std::smatch match;
			std::regex_search(line, match, constants::re_pattern_lines);
			if (match.empty())
			{
				//parse error
				throw ParseError(line_count);
			}
			//match[1] represents the line name
			this->transit_lines_[match[1]] = Line(match);
		}
	}
	else
	{
		throw FileReadError();
	}
}

void TransitNetwork::output_stop_info(const int stop_number)
{
	if(!stops_.contains(stop_number))
	{
		throw UnrecognizedStop();
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
		output.close();
	}
	else
	{
		throw FileWriteError();
	}
	
}

void TransitNetwork::output_line_info(const std::string& line_name)
{
	//add a check if line does not exists
	if (!transit_lines_.contains(line_name))
	{
		throw UnrecognizedLine();
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
		output.close();
	}
	else
	{
		throw FileWriteError();
	}
	
}

void TransitNetwork::output_line_statistics(const std::string& line_name)
{
	if (!transit_lines_.contains(line_name))
	{
		throw UnrecognizedLine();
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
		output.close();
	}
	else
	{
		throw FileWriteError();
	}
}

void TransitNetwork::find_path(const constants::strategy strategy, const int departure_stop_number,
	const int destination_stop_number, const std::string& departure_time_string)
{
	std::smatch match;
	std::regex_search(departure_time_string, match, constants::re_pattern_time);
	if(match.empty())
	{
		throw TimeError();
	}
	int departure_time = std::stoi(match[1]) * 60 + std::stoi(match[2]);

	if (!stops_.contains(departure_stop_number))
	{
		throw UnrecognizedDepartureStop();
	}
	if (!stops_.contains(destination_stop_number))
	{
		throw UnrecognizedDestinationStop();
	}
	if (departure_stop_number == destination_stop_number)
	{
		std::cout << constants::console_green_text << "Već ste na odredištu. Fajl nije kreiran."
		<< "\n" << constants::console_green_text;
		return;
	}

	std::size_t solved_nodes_count = 0;
	std::string departure_stop = std::to_string(departure_stop_number);
	std::string destination_stop = std::to_string(destination_stop_number);
	int distance_running_total = 0;
	int current_time = departure_time;
	fill_adjacency_map(strategy);

	//initialize traversal_map
	for (auto& [stop, adjacent_stops] : graph_adjacency_map_)
	{
		int distance = constants::infinity;
		if (stop == departure_stop)
		{
			distance = 0;
		}
		graph_traversal_map_[stop] = { distance, false, "" };
	}

	//Dijkstra's algorithm implementation
	while (solved_nodes_count < graph_traversal_map_.size())
	{
		std::string nearest_stop = find_nearest_stop();

		distance_running_total = std::get<0>(graph_traversal_map_[nearest_stop]);
		current_time = departure_time + distance_running_total;

		std::get<1>(graph_traversal_map_[nearest_stop]) = true;
		solved_nodes_count++;

		// end of path-finding
		if (nearest_stop == destination_stop)
		{
			std::vector<std::string> path;
			std::string parent = std::get<2>(graph_traversal_map_[destination_stop]);
			while (!parent.empty())
			{
				if (parent.find('_') != std::string::npos)
				{
					path.push_back(parent);
				}
				parent = std::get<2>(graph_traversal_map_[parent]);
			}
			std::reverse(path.begin(), path.end());

			if (path.empty())
			{
				std::cout << constants::console_magenta_text << "Putanja nije pronađena!\n" << constants::console_default;
				return;
			}
			output_path("putanja_" + std::to_string(departure_stop_number) + '_' + std::to_string(destination_stop_number) + ".txt", path);

			std::cout << constants::console_green_text;
			std::cout <<  "Putanja je pronađena!" << '\n';
			std::cout << "Trajanje putanje: " << distance_running_total % constants::impossible_weight << " min" << '\n';
			std::cout << "Broj presedanja : " << number_of_transfers(path) << '\n';
			std::cout << "Generisan je fajl putanja_" << departure_stop << "_"
				<< destination_stop << ".txt sa putanjom između stanica " << departure_stop << " i " << destination_stop << "." << '\n';
			std::cout << constants::console_default;

			return;
		}

		//checking all neighbor stops
		for (auto& [stop_number, relative_distance] : graph_adjacency_map_[nearest_stop])
		{
			//check to see if the current stop is a main stop(one where transfers could be performed)
			if (nearest_stop.find('_') == std::string::npos)
			{
				//find first departures of all lines on this stop
				auto node_timetable = find_first_departure_after(std::stoi(nearest_stop), current_time % constants::impossible_weight);
				//parse the stop_number to get the line name
				std::smatch match;
				std::regex_search(stop_number, match, constants::re_pattern_pathfinding_stop_name);
				//check to see if a departure exists
				if (node_timetable.contains(match[2]))
				{
					relative_distance = node_timetable.at(match[2]);
				}
				else
				{
					//no departure, stop gets an infinite distance
					std::get<0>(graph_traversal_map_[stop_number]) = constants::infinity;
					continue;
				}
			}
			//check if current path is the fastest
			if (distance_running_total < std::get<0>(graph_traversal_map_[stop_number]))
			{
				std::get<0>(graph_traversal_map_[stop_number]) = distance_running_total + relative_distance;
				std::get<2>(graph_traversal_map_[stop_number]) = nearest_stop;
			}
		}
	}
	std::cout << constants::console_magenta_text << "Putanja nije pronađena!\n" << constants::console_default;
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
	/*
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
	*/
}

void TransitNetwork::fill_adjacency_map(const constants::strategy strategy)
{
	//if the fastest option is chosen, this distance becomes 0
	//if the least_transfers option is chosen, this distance 
	//becomes a very large number to discourage transfers
	int stop_transfer_distance;
	switch (strategy)
	{
	case constants::strategy::FASTEST:
		stop_transfer_distance = 0;
		break;
	case constants::strategy::LEAST_TRANSFERS:
		stop_transfer_distance = constants::impossible_weight;
		break;
	}
	
	for (auto& [line_name, line]: transit_lines_)
	{
		auto stops = line.get_stops();
		for (std::size_t i = 0; i < stops.size(); i++)
		{
			if (i != stops.size() - 1)
			{
				//distance between current sub-stop and next sub-stop of the same line (3 minutes)
				graph_adjacency_map_[std::to_string(stops[i]) + "_" + line_name][std::to_string(stops[i + 1]) + "_" + line_name] =  constants::inter_stop_distance;
			}
			//distance between sub-stops and main stops (controls the number of transfers)
			graph_adjacency_map_[std::to_string(stops[i]) + "_" + line_name][std::to_string(stops[i])] = stop_transfer_distance; 
		}
	}
	for (auto& [stop, stop_name] : stops_)
	{
		for (auto& found_line : find_stop_lines(stop))
		{
			//distance between main stops and sub-stops (always 0, later changed to waiting time)
			graph_adjacency_map_[std::to_string(stop)][std::to_string(stop) + "_" + found_line] = 0;
		}
	}
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

std::string TransitNetwork::find_nearest_stop()
{
	int minimal_distance = constants::infinity;
	std::string result = graph_traversal_map_.begin()->first;

	for (auto& [stop_name, stop_tuple] : graph_traversal_map_)
	{
		if (!std::get<1>(stop_tuple)) {
			const int distance = std::get<0>(stop_tuple);
			if (distance < minimal_distance)
			{
				minimal_distance = distance;
				result = stop_name;
			}
		}
	}
	return result;
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
				lines_first_departure_after[line] = timetable_it->first - time;
			}
		}
		
		++timetable_it;
	}


	return lines_first_departure_after;
}



void TransitNetwork::output_path(const std::string& filename, const std::vector<std::string>& path) const
{
	std::ofstream output;
	output.open(filename);
	if (output.is_open()) {
		std::string old_line;
		std::string current_line;
		for (std::size_t i = 0; i < path.size(); i++)
		{
			std::smatch match;
			std::regex_search(path[i], match, constants::re_pattern_pathfinding_stop_name);
			old_line = current_line;
			current_line = match[2];

			if (old_line != current_line) {
				if (i != 0) {
					output << '\n';
				}
				output << old_line << "->" << current_line << '\n';
			}
			output << match[1];
			if (i != path.size() - 1)
			{
				std::regex_search(path[i + 1], match, constants::re_pattern_pathfinding_stop_name);
				if (match[2] == current_line)
				{
					output << ' ';
				}
			}
		}
	}
}

int TransitNetwork::number_of_transfers(const std::vector<std::string>& path) const
{
	if(path.empty())
	{
		return 0;
	}
	std::set<std::string> lines;
	for (auto& stop : path)
	{
		std::smatch match;
		std::regex_search(stop, match, constants::re_pattern_pathfinding_stop_name);
		lines.insert(match[2]);
	}
	return lines.size() - 1;
}