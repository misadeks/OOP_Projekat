#ifndef TRANSIT_NETWORK
#define TRANSIT_NETWORK

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Line.h"

class TransitNetwork
{
public:
	void load_data(const std::string& stops_filename, const std::string& transit_lines_filename);
	std::map<std::string, Line> transit_lines;
	std::map<int, std::string> stops;
private:
	
};
#endif

