#include "Reader.hpp"
#include <iostream>
#include <stdexcept>

std::string Reader::ReadLine()
{
	std::string line;
	if (file.is_open())
	{
		getline(file, line);
	}
	else
		throw std::runtime_error("There is no file opened!");

	return line;
}

void Reader::Reset()
{
	file.clear();
	file.seekg(0);
}

void Reader::SkipUntilNot(char skip)
{
	if (file.peek() == '\n')
		ReadLine();

	std::streampos prev = file.tellg();
	while (ReadLine().at(0) == skip)
	{
		prev = file.tellg();
	}
	file.seekg(prev);
}
