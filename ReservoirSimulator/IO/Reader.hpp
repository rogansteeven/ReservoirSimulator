#pragma once

#include <string>
#include <fstream>

class Reader
{
public:
	Reader(const std::string& filepath) : file(filepath) {}

	std::string ReadLine();
	std::string ReadLine(int n, bool reset = true);

	void Reset();
	void Skip(char skipChar);

	std::ifstream& GetFile();

private:
	std::ifstream file;
};