#pragma once

#include "../Data/Properties.hpp"
#include "../IO/Reader.hpp"
#include <string>

class PVT
{
public:
	PVT(const std::string& filepath);

	Reservoir ReadReservoirData();
	std::vector<RockPhysic> ReadRockPhysics();

private:
	Reader reader;
	PVTData data;
};