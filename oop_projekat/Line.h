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
	explicit Line(const std::smatch& line_data);
	std::vector<int> stops_;
private:
	std::string line_name_;
	
	int first_departure_time_ = 0;
	int last_departure_time_ = 0;
	int interval_ = 0;

	std::vector<int> split_stops(const std::string& stops_string) const;
};
#endif

