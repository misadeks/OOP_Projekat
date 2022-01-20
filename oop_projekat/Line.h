#ifndef LINE
#define LINE

#include <string>
#include <sstream>
#include <vector>
#include <regex>

class Line
{
public:
	Line() = default;
	Line(const Line&) = default;
	~Line() = default;

	explicit Line(const std::smatch& line_data);

	std::vector<int> get_stops();
	const int number_of_departures();

	const int get_first_departure_time();
	const int get_last_departure_time();
	const int get_interval();
private:
	std::string line_name_;
	std::vector<int> stops_;
	int first_departure_time_ = 0;
	int last_departure_time_ = 0;
	int interval_ = 0;

	std::vector<int> split_stops(const std::string& stops_string) const;
};
#endif

