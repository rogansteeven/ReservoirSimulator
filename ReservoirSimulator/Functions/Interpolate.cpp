#include "Interpolate.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

float Interpolate(std::vector<float> tabx, std::vector<float> taby, float x)
{
	size_t sizeX = tabx.size();
	size_t sizeY = taby.size();

	// Check size
	if (sizeX != sizeY)
	{
		std::cout << "Error size\n";
		throw std::runtime_error("Size of table x and table y is not the same!");
	}

	// Check boundary
	if (x < tabx[0])
		return taby[0];

	if (x > tabx[sizeX - 1])
		return taby[sizeX - 1];

	// Check value inbetween
	int count = 0;
	for (const auto& entry : tabx)
	{
		if (x == entry)
			return taby[count];

		if (x < entry)
			break;

		count++;
	}

	// Interpolate
	return taby[count - 1] + (x - tabx[count - 1]) * (taby[count] - taby[count - 1]) / (tabx[count] - tabx[count - 1]);
}