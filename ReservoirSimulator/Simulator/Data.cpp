#include "Data.hpp"
#include <iostream>

std::shared_ptr<Data> Data::Create(const std::string& filepath)
{
	return std::make_shared<Data>(filepath);
}

Data::Data(const std::string& filepath)
	: m_Reader(filepath)
{
	ReadInitialReservoirData();
	ReadRockPhysicData();
	ReadDensityData();
	ReadPVTData();
}

void Data::ReadInitialReservoirData()
{
	// Read Number of Grids
	m_Reader.SkipUntilNot('/');
	m_Reader >> m_Content.reservoir.nx;
	m_Reader >> m_Content.reservoir.ny;
	m_Reader >> m_Content.reservoir.nz;

	// Read Dimension in x, y, z direction
	m_Reader.SkipUntilNot('/');
	m_Reader >> m_Content.reservoir.tx;
	m_Reader >> m_Content.reservoir.ty;
	m_Reader >> m_Content.reservoir.tz;

	// Read initial pressure and water saturation
	m_Reader.SkipUntilNot('/');
	m_Reader >> m_Content.reservoir.pinit;
	m_Reader >> m_Content.reservoir.swi;

	// Read porosity, compressibility, ref. pressure
	m_Reader.SkipUntilNot('/');
	m_Reader >> m_Content.reservoir.phi0;
	m_Reader >> m_Content.reservoir.crock;
	m_Reader >> m_Content.reservoir.pref;

	// Read permeability in x, y, z direction
	m_Reader.SkipUntilNot('/');
	m_Reader >> m_Content.reservoir.permx;
	m_Reader >> m_Content.reservoir.permy;
	m_Reader >> m_Content.reservoir.permz;
}

void Data::ReadRockPhysicData()
{
	int n;
	float sw, krw, kro, pcow;

	// Read no of entries
	m_Reader.SkipUntilNot('/');
	m_Reader >> n;

	// Read all rock physic data
	m_Reader.SkipUntilNot('/');
	while (m_Reader >> sw >> krw >> kro >> pcow) // break the loop by failing ifstream
	{
		m_Content.rockPhysic.push_back({ sw, krw, kro, pcow });
	}
	m_Reader.Clear(); // clear the fail flag as it was intended
}

void Data::ReadDensityData()
{
	// Read density for oil, gas, and water
	m_Reader.SkipUntilNot('/');
	m_Reader >> m_Content.density.oil;
	m_Reader >> m_Content.density.gas;
	m_Reader >> m_Content.density.water;
}

void Data::ReadPVTData()
{
	int n;

	// Read no of entries
	m_Reader.SkipUntilNot('/');
	m_Reader >> n;

	ReadPVTOData();
	ReadPVTGData();
	ReadPVTWData();
}

void Data::ReadPVTOData()
{
	float rs, p, bo, viso;

	// Read PVTO Data
	m_Reader.SkipUntilNot('/');
	while (m_Reader >> rs >> p >> bo >> viso) // break the loop by failing ifstream
	{
		m_Content.pvto.push_back({ rs, p, bo, viso });
	}
	m_Reader.Clear(); // clear the fail flag as it was intended
}

void Data::ReadPVTGData()
{
	float p, bg, visg;

	// Read PVTO Data
	m_Reader.SkipUntilNot('/');
	while (m_Reader >> p >> bg >> visg) // break the loop by failing ifstream
	{
		m_Content.pvtg.push_back({ p, bg, visg });
	}
	m_Reader.Clear(); // clear the fail flag as it was intended
}

void Data::ReadPVTWData()
{
	float pref, bwref, cwref, viswref, viscosibility;

	// Read PVTO Data
	m_Reader.SkipUntilNot('/');
	while (m_Reader >> pref >> bwref >> cwref >> viswref >> viscosibility) // break the loop by failing ifstream
	{
		m_Content.pvtw.push_back({ pref, bwref, cwref, viswref, viscosibility });
	}
	m_Reader.Clear(); // clear the fail flag as it was intended
}