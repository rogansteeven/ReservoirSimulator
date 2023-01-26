#include "PVT.hpp"

PVT::PVT(const std::string& filepath)
	: reader(filepath)
{
	ReadReservoirData();
}

Reservoir PVT::ReadReservoirData()
{
	// Get grids
	reader.Skip('/');
	reader.GetFile() >> data.reservoir.nx >> data.reservoir.ny >> data.reservoir.nz;

	// Get dimensions
	reader.Skip('/');
	reader.GetFile() >> data.reservoir.tx >> data.reservoir.ty >> data.reservoir.tz;

	// Get Pi, Swi
	reader.Skip('/');
	reader.GetFile() >> data.reservoir.pi >> data.reservoir.swi;

	// Get Phi0, Crock, Pref
	reader.Skip('/');
	reader.GetFile() >> data.reservoir.phi0 >> data.reservoir.crock >> data.reservoir.pref;

	// Get Permeability
	reader.Skip('/');
	reader.GetFile() >> data.reservoir.permx >> data.reservoir.permy >> data.reservoir.permz;
}

std::vector<RockPhysic> PVT::ReadRockPhysics()
{
	// Get number of entries
	int n;
	reader.Skip('/');
	reader.GetFile() >> n;

	// Get rock physics
	float Sw;    
	float Kw, Ko;
	float Cow;   
	reader.Skip('/');
	for (int i = 0; i < n; i++)
	{
		reader.GetFile() >> Sw >> Kw >> Ko >> Cow;
		data.rockPhysic.push_back({ Sw, Kw, Ko, Cow });
	}
}
