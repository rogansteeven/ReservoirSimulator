#include "PVT.hpp"
#include <iostream>

PVT::PVT(const std::string& filepath)
	: reader(filepath)
{
	Init();
	//std::cout << reader.ReadLine();
}

void PVT::Init()
{
	ReadReservoirData();
	ReadRockPhysics();
	ReadDensity();
	ReadPVT();
}

void PVT::ReadReservoirData()
{
	// Get grids
	reader.SkipLeading('/');
	reader.GetFile() >> data.reservoir.nx >> data.reservoir.ny >> data.reservoir.nz;
	reader.ReadLine(); // Change pointer to the next line

	// Get dimensions
	reader.SkipLeading('/');
	reader.GetFile() >> data.reservoir.tx >> data.reservoir.ty >> data.reservoir.tz;
	reader.ReadLine(); // Change pointer to the next line

	// Get Pi, Swi
	reader.SkipLeading('/');
	reader.GetFile() >> data.reservoir.pi >> data.reservoir.swi;
	reader.ReadLine(); // Change pointer to the next line

	// Get Phi0, Crock, Pref
	reader.SkipLeading('/');
	reader.GetFile() >> data.reservoir.phi0 >> data.reservoir.crock >> data.reservoir.pref;
	reader.ReadLine(); // Change pointer to the next line

	// Get Permeability
	reader.SkipLeading('/');
	reader.GetFile() >> data.reservoir.permx >> data.reservoir.permy >> data.reservoir.permz;
	reader.ReadLine(); // Change pointer to the next line
}

void PVT::ReadRockPhysics()
{
	// Get number of entries
	int n;
	reader.SkipLeading('/');
	reader.GetFile() >> n;
	reader.ReadLine(); // Change pointer to the next line

	// Get rock physics
	float Sw, Kw, Ko, Cow;
	reader.SkipLeading('/');
	while (reader.GetFile() >> Sw >> Kw >> Ko >> Cow)
	{
		data.rockPhysic.push_back({ Sw, Kw, Ko, Cow });
	}
	reader.Clear();
	reader.ReadLine(); // Change pointer to the next line
}

void PVT::ReadDensity()
{
	// Get density
	reader.SkipLeading('/');
	reader.GetFile() >> data.density.oil >> data.density.gas >> data.density.water;
	reader.ReadLine(); // Change pointer to the next line
}

void PVT::ReadPVT()
{
	// Get number of entries
	int n;
	reader.SkipLeading('/');
	reader.GetFile() >> n;
	reader.ReadLine(); // Change pointer to the next line

	ReadPVTO();
	ReadPVTG();
	ReadPVTW();
}

void PVT::ReadPVTO()
{
	// Get PVT Oil
	reader.SkipLeading('/');
	float Rs, Po, Bo, Viso;
	while (reader.GetFile() >> Rs >> Po >> Bo >> Viso)
	{
		data.pvto.push_back({ Rs, Po, Bo, Viso });
	}
	reader.Clear();
	reader.ReadLine(); // Change pointer to the next line
}

void PVT::ReadPVTG()
{
	// Get PVT Gas
	reader.SkipLeading('/');
	float Pg, Bg, Vg; 
	while (reader.GetFile() >> Pg >> Bg >> Vg)
	{
		data.pvtg.push_back({ Pg, Bg, Vg });
	}
	reader.Clear();
	reader.ReadLine(); // Change pointer to the next line
}

void PVT::ReadPVTW()
{
	// Get PVT Water
	reader.SkipLeading('/');
	float Pref, Bwre, Cwre, Visw, Visbw; 
	while (reader.GetFile() >> Pref >> Bwre >> Cwre >> Visw >> Visbw)
	{
		data.pvtw.push_back({ Pref, Bwre, Cwre, Visw, Visbw });
	}
	reader.Clear();
	reader.ReadLine(); // Change pointer to the next line
}
