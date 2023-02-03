#pragma once

#include "DataType.hpp"
#include "../IO/Reader.hpp"
#include "Typedef.hpp"

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

		std::vector<WellData> wells;
	};

public:
	Data(const DataSourceMap& sourceMap);

	const Content& GetContent() const { return m_Content; }

	static std::shared_ptr<Data> Create(const DataSourceMap& sourceMap);

private:
	void ReadProject2Data(const std::string& filepath);
	void ReadProject4Data(const std::string& filepath);

private:
	void ReadInitialReservoirData(Reader& reader);
	void ReadRockPhysicData(Reader& reader);
	void ReadDensityData(Reader& reader);
	void ReadPVTData(Reader& reader);
	void ReadPVTOData(Reader& reader);
	void ReadPVTGData(Reader& reader);
	void ReadPVTWData(Reader& reader);

	void ReadWellData(Reader& reader);

private:
	Content m_Content;
};