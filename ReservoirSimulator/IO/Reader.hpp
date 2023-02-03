#pragma once

#include <string>
#include <fstream>

class Reader
{
public:
	Reader(const std::string& filepath) : file(filepath) {}
	~Reader() { file.close(); }

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
	void Close() { file.close(); }

private:
	std::ifstream file;
};