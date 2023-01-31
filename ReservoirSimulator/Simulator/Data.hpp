#pragma once

#include "DataType.hpp"
#include "../IO/Reader.hpp"

#include <memory>
#include <string>
#include <vector>

class Data
{
public:
	struct Content
	{
		ReservoirData reservoir;
		DensityData density;

		std::vector<RockPhysicData> rockPhysic;
		std::vector<PVTOData> pvto;
		std::vector<PVTGData> pvtg;
		std::vector<PVTWData> pvtw;
	};

public:
	Data(const std::string& filepath);

	const Content& GetContent() const { return m_Content; }

	static std::shared_ptr<Data> Create(const std::string& filepath);

private:
	void ReadInitialReservoirData();
	void ReadRockPhysicData();
	void ReadDensityData();
	void ReadPVTData();
	void ReadPVTOData();
	void ReadPVTGData();
	void ReadPVTWData();

private:
	Content m_Content;
	Reader m_Reader;
};