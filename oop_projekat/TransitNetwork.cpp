#include "TransitNetwork.h"


void TransitNetwork::load_stops_data(const std::string& stops_filename)
{
	std::ifstream stops_input(stops_filename);

	if (stops_input.is_open())
	{
		std::string line;
		while (std::getline(stops_input, line))
		{
			try {
				std::smatch match;
				//match the regex pattern
				std::regex_search(line, match, constants::re_pattern_stops);
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
}

void TransitNetwork::load_transit_lines_data(const std::string& transit_lines_filename)
{
	std::ifstream lines_input(transit_lines_filename);

	if (lines_input.is_open())
	{
		std::string line;
		while (std::getline(lines_input, line))
		{
			try {
				std::smatch match;
				std::regex_search(line, match, constants::re_pattern_lines);
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
		output.close();
	}
	else
	{
		//error file output
	}
	
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
		output.close();
	}
	else
	{
		//error file
	}
	
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
		output.close();
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

void TransitNetwork::fill_adjacency_map()
{
	for (auto& [line_name, line]: transit_lines_)
	{
		auto stops = line.get_stops();
		for (std::size_t i = 0; i < stops.size(); i++)
		{
			if (i != stops.size() - 1)
			{
				graph_adjacency_map_[std::to_string(stops[i]) + "_" + line_name][std::to_string(stops[i + 1]) + "_" + line_name] =  3;
				 // change to smth for the third strategy
			}
			graph_adjacency_map_[std::to_string(stops[i]) + "_" + line_name][std::to_string(stops[i])] = 0; // change to smth for the third strategy
		}
	}
	for (auto& [stop, stop_name] : stops_)
	{
		for (auto& found_line : find_stop_lines(stop))
		{
			graph_adjacency_map_[std::to_string(stop)][std::to_string(stop) + "_" + found_line] = 0; // change to smth for the third strategy
		}
	}
	// for (auto& [stop_id, adjacent] : graph_adjacency_map_)
	// {
	// 	std::cout << stop_id << " : ";
	// 	for (auto& [line_name, distance] : adjacent)
	// 	{
	// 		std::cout << line_name <<":"  << distance << " ";
	// 	}
	// 	std::cout << '\n';
	// }
	
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

std::string TransitNetwork::find_nearest()
{
	int minimal_distance = constants::infinity;
	std::string result = graph_traversal_map.begin()->first;

	for (auto& [stop_name, stop_tuple] : graph_traversal_map)
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


std::vector<std::string> TransitNetwork::find_path_fastest(const int departure_stop_number, const int destination_stop_number, const int departure_time)
{
	std::size_t solved_nodes_count = 0;
	std::string departure_stop = std::to_string(departure_stop_number);
	std::string destination_stop = std::to_string(destination_stop_number);
	int distance_running_total = 0;
	int current_time = departure_time;
	fill_adjacency_map();

	//initialize traversal_map
	for (auto& [stop, adjacent_stops] : graph_adjacency_map_)
	{
		int distance = constants::infinity;
		if(stop == departure_stop)
		{
			distance = 0;
		}
		graph_traversal_map[stop] = { distance, false, ""};
	}


	while(solved_nodes_count < graph_traversal_map.size())
	{
		std::string nearest_node = find_nearest();

		distance_running_total = std::get<0>(graph_traversal_map[nearest_node]);
		current_time = departure_time + distance_running_total;

		std::get<1>(graph_traversal_map[nearest_node]) = true;
		solved_nodes_count++;

		if (nearest_node == destination_stop)
		{
			//std::cout << "kuku majko nesto sam nasao!" << '\n';
			/*
			for (auto& [a, b] : graph_traversal_map)
			{
				std::cout << a << " " << std::get<0>(b) << " " << std::get<1>(b) << " " << std::get<2>(b) << '\n';
			}
			return;
			*/
			//ako je dosao do kraja


			std::vector<std::string> route_stops;
			std::string parent = std::get<2>(graph_traversal_map[destination_stop]);
			while (!parent.empty())
			{
				if(parent.find("_") != std::string::npos)
				{
					route_stops.push_back(parent);
				}
				parent = std::get<2>(graph_traversal_map[parent]);
			}
			std::reverse(route_stops.begin(), route_stops.end());
			std::cout << "Duration: " << distance_running_total % constants::impossible_weight << " " << distance_running_total << '\n';
			return route_stops;
		}

		
		for(auto& [stop_number, relative_distance] : graph_adjacency_map_[nearest_node])
		{
			if (nearest_node.find("_") == std::string::npos)
			{
				auto node_timetable = find_first_departure_after(std::stoi(nearest_node), current_time % constants::impossible_weight);
				std::smatch match;
				std::regex_search(stop_number, match, constants::re_pattern_pathfinding_stop_name);
				if(node_timetable.contains(match[2]))
				{
					relative_distance = node_timetable.at(match[2]);
				}
				else
				{
					std::get<0>(graph_traversal_map[stop_number]) = constants::infinity;
					continue;
				}
				
			}
			if(distance_running_total < std::get<0>(graph_traversal_map[stop_number]))
			{
				std::get<0>(graph_traversal_map[stop_number]) = distance_running_total + relative_distance;
				std::get<2>(graph_traversal_map[stop_number]) = nearest_node;
			}
			
		}

	}
	
	/*
	for (auto& [a, b] : graph_traversal_map)
	{
		std::cout << a << " " << std::get<0>(b) << " " << std::get<1>(b) << " " << std::get<2>(b) << '\n';
	}
	return;
	*/
}
