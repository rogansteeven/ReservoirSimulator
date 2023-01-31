#pragma once

#include <string>
#include <fstream>

class Reader
{
public:
	Reader(const std::string& filepath) : file(filepath) {}

	std::string ReadLine();
	void SkipUntilNot(char skip);
	
	void Reset();

public:
	template<typename T>
	std::ifstream& operator>>(T& val)
	{
		file >> val;
		return file;
	}

public:
	void Clear() { file.clear(); }

private:
	std::ifstream file;
};