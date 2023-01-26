#include "Reader.hpp"

std::string Reader::ReadLine()
{
	std::string line;
	if (file.is_open())
	{
		getline(file, line);
	}
	return line;
}

std::string Reader::ReadLine(int n, bool reset)
{
	if (reset)
		Reset();

	std::string line;
	if (file.is_open())
	{
		for (int i = 0; i < n; i++)
			getline(file, line);
	}
	return line;
}

void Reader::Reset()
{
	file.clear();
	file.seekg(0);
}

std::ifstream& Reader::GetFile()
{
	return file;
}
