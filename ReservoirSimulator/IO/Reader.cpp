#include "Reader.hpp"
#include <iostream>

std::string Reader::ReadLine()
{
	std::string line;
	if (file.is_open())
	{
		getline(file, line);
	}
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
