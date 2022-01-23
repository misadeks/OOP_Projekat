#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <exception>
#include <string>
#include <sstream>

#include "Constants.h"

class Exception : public std::exception
{
public:
	Exception() : std::exception("Exception") {}
	virtual const std::string repr_string() = 0;
};

class FileReadError : public Exception
{
public:
	FileReadError() : Exception() {}
	const std::string repr_string() override;
};

class FileWriteError : public Exception
{
public:
	FileWriteError() : Exception() {}
	const std::string repr_string() override;
};

class ParseError : public Exception
{
public:
	explicit ParseError(const int line_number) : Exception(), line_number_(line_number) {}

	const std::string repr_string() override;
private:
	int line_number_;
};

class UnrecognizedStop: public Exception
{
public:
	UnrecognizedStop() : Exception() {}
	const std::string repr_string() override;
};

class UnrecognizedLine : public Exception
{
public:
	UnrecognizedLine() : Exception() {}
	const std::string repr_string() override;
};

class UnrecognizedDepartureStop : public Exception
{
public:
	UnrecognizedDepartureStop() : Exception() {}
	const std::string repr_string() override;
};

class UnrecognizedDestinationStop : public Exception
{
public:
	UnrecognizedDestinationStop() : Exception() {}
	const std::string repr_string() override;
};

class TimeError : public Exception
{
public:
	TimeError() : Exception() {}
	const std::string repr_string() override;
};
#endif


