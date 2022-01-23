#include "Exceptions.h"

const std::string FileReadError::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Fajl nije pronađen." << constants::console_default;
	return ss.str();
}

const std::string FileWriteError::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Nije moguće ispisati fajl." << constants::console_default;
	return ss.str();
}

const std::string ParseError::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Podaci na liniji " << line_number_ << " fajla nisu u odgovarajućem formatu." << constants::console_default;
	return ss.str();
}

const std::string UnrecognizedStop::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Uneto stajalište nije prepoznato." << constants::console_default;
	return ss.str();
}

const std::string UnrecognizedLine::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Uneta linija nije prepoznata." << constants::console_default;
	return ss.str();
}

const std::string UnrecognizedDepartureStop::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Uneto početno stajalište nije prepoznato." << constants::console_default;
	return ss.str();
}

const std::string UnrecognizedDestinationStop::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Uneto krajnje stajalište nije prepoznato." << constants::console_default;
	return ss.str();
}

const std::string TimeError::repr_string()
{
	std::stringstream ss;

	ss << constants::console_red_text << "GREŠKA: Uneto vreme nije u odgovarajućem formatu." << constants::console_default;
	return ss.str();
}
