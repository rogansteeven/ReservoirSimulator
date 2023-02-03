#include "Data.hpp"
#include <iostream>

std::shared_ptr<Data> Data::Create(const DataSourceMap& sourceMap)
{
	return std::make_shared<Data>(sourceMap);
}

Data::Data(const DataSourceMap& sourceMap)
{
	for (const auto& [prop, source] : sourceMap)
	{
		switch (prop)
		{
		case DataSource::P2:
			ReadProject2Data(source); break;
		case DataSource::P4:
			ReadProject4Data(source); break;
		default:
			throw std::runtime_error("Unknown data source!");
		}
	}
}

void Data::ReadProject2Data(const std::string& filepath)
{
	Reader reader = Reader(filepath);
	ReadInitialReservoirData(reader);
	ReadRockPhysicData(reader);
	ReadDensityData(reader);
	ReadPVTData(reader);
}

void Data::ReadProject4Data(const std::string& filepath)
{
	Reader reader = Reader(filepath);
	ReadWellData(reader);
}

void Data::ReadInitialReservoirData(Reader& reader)
{
	// Read Number of Grids
	reader.SkipUntilNot('/');
	reader >> m_Content.reservoir.nx;
	reader >> m_Content.reservoir.ny;
	reader >> m_Content.reservoir.nz;

	// Read Dimension in x, y, z direction
	reader.SkipUntilNot('/');
	reader >> m_Content.reservoir.tx;
	reader >> m_Content.reservoir.ty;
	reader >> m_Content.reservoir.tz;

	// Read initial pressure and water saturation
	reader.SkipUntilNot('/');
	reader >> m_Content.reservoir.pinit;
	reader >> m_Content.reservoir.swi;

	// Read porosity, compressibility, ref. pressure
	reader.SkipUntilNot('/');
	reader >> m_Content.reservoir.phi0;
	reader >> m_Content.reservoir.crock;
	reader >> m_Content.reservoir.pref;

	// Read permeability in x, y, z direction
	reader.SkipUntilNot('/');
	reader >> m_Content.reservoir.permx;
	reader >> m_Content.reservoir.permy;
	reader >> m_Content.reservoir.permz;
}

void Data::ReadRockPhysicData(Reader& reader)
{
	int n;
	float sw, krw, kro, pcow;

	// Read no of entries
	reader.SkipUntilNot('/');
	reader >> n;

	// Read all rock physic data
	reader.SkipUntilNot('/');
	while (reader >> sw >> krw >> kro >> pcow) // break the loop by failing ifstream
	{
		m_Content.rockPhysic.push_back({ sw, krw, kro, pcow });
	}
	reader.Clear(); // clear the fail flag as it was intended
}

void Data::ReadDensityData(Reader& reader)
{
	// Read density for oil, gas, and water
	reader.SkipUntilNot('/');
	reader >> m_Content.density.oil;
	reader >> m_Content.density.gas;
	reader >> m_Content.density.water;
}

void Data::ReadPVTData(Reader& reader)
{
	int n;

	// Read no of entries
	reader.SkipUntilNot('/');
	reader >> n;

	ReadPVTOData(reader);
	ReadPVTGData(reader);
	ReadPVTWData(reader);
}

void Data::ReadPVTOData(Reader& reader)
{
	float rs, p, bo, viso;

	// Read PVTO Data
	reader.SkipUntilNot('/');
	while (reader >> rs >> p >> bo >> viso) // break the loop by failing ifstream
	{
		m_Content.pvto.push_back({ rs, p, bo, viso });
	}
	reader.Clear(); // clear the fail flag as it was intended
}

void Data::ReadPVTGData(Reader& reader)
{
	float p, bg, visg;

	// Read PVTO Data
	reader.SkipUntilNot('/');
	while (reader >> p >> bg >> visg) // break the loop by failing ifstream
	{
		m_Content.pvtg.push_back({ p, bg, visg });
	}
	reader.Clear(); // clear the fail flag as it was intended
}

void Data::ReadPVTWData(Reader& reader)
{
	float pref, bwref, cwref, viswref, viscosibility;

	// Read PVTO Data
	reader.SkipUntilNot('/');
	while (reader >> pref >> bwref >> cwref >> viswref >> viscosibility) // break the loop by failing ifstream
	{
		m_Content.pvtw.push_back({ pref, bwref, cwref, viswref, viscosibility });
	}
	reader.Clear(); // clear the fail flag as it was intended
}

void Data::ReadWellData(Reader& reader)
{
	// Read number of well
	int n;
	reader.SkipUntilNot('/');
	reader >> n;

	// Read well coordinate
	int i, j, k;
	reader.SkipUntilNot('/');
	while (reader >> i >> j >> k) // break the loop by failing ifstream
		m_Content.wells.push_back({ i - 1, j - 1, k - 1 });
	reader.Clear(); // clear the fail flag as it was intended

	// Read number of rate
	int v;
	reader.SkipUntilNot('/');
	reader >> v;

	// Read time and rate
	float time, rate;
	for (int i = 0; i < n; i++)
	{
		reader.SkipUntilNot('/');
		while (reader >> time >> rate)
			m_Content.wells[i].timeRate.push_back({ time, -rate });
		reader.Clear();
	}
}