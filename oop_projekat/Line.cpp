#include "Line.h"


Line::Line(const std::smatch& line_data)
{
	line_name_ = line_data[1];
	
	first_departure_time_ = std::stoi(line_data[2]) * 60 + std::stoi(line_data[3]);
	last_departure_time_ = std::stoi(line_data[4]) * 60 + std::stoi(line_data[5]);
	interval_ = std::stoi(line_data[6]);

	stops_ = split_stops(line_data[7]);
}

std::vector<int> Line::get_stops()
{
	return stops_;
}

const int Line::number_of_departures()
{
	return (last_departure_time_ - first_departure_time_) / interval_ + 1;
}

const int Line::get_first_departure_time()
{
	return first_departure_time_;
}

const int Line::get_last_departure_time()
{
	return last_departure_time_;
}

const int Line::get_interval()
{
	return interval_;
}

std::vector<int> Line::split_stops(const std::string& stops_string) const
{
	std::string temp;
	std::stringstream ss(stops_string);
	std::vector<int> stops;

	while (getline(ss, temp, ' ')) {
		stops.push_back(stoi(temp));
	}
	return stops;
}
