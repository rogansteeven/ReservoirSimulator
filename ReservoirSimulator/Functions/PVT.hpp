#pragma once

#include "../Data/Properties.hpp"
#include "../IO/Reader.hpp"
#include <memory>

class PVT
{
public:
	PVT(const std::string& filepath);

	void Init();

	void ReadReservoirData();
	void ReadRockPhysics();
	void ReadDensity();

	void ReadPVT();
	void ReadPVTO();
	void ReadPVTG();
	void ReadPVTW();

	const Data GetData() const { return data; }

	static std::unique_ptr<PVT> Create(const std::string& filepath) { return std::make_unique<PVT>(filepath); }

private:
	Reader reader;
	Data data;
};