#pragma once
#include <vector>

enum Props
{
	Bo = 0, Viso, Rs, Bw, Visw
};

struct Density // [lb/cuft]
{
	float oil, gas, water; // at reference condition
};

struct PVTO
{
	float Rs;   // [Mscf/stb]
	float Po;   // [psia]
	float Bo;   // [rb/stb]
	float Viso; // [cp]
};

struct PVTG
{
	float Pg; // [psia]
	float Bg; // [rb/Mscf]
	float Vg; // [cp]
};

struct PVTW
{
	float Pref;  // [psia]
	float Bwref; // [rb/stb]
	float Cwref; // [1/psia]
	float Visw;  // [cp]
	float Visbw; // [1/psia]
};

struct RockPhysic
{
	float Sw;     // Water saturation [.frac]
	float Kw, Ko; // Rel. Perm. Water [.frac], Rel. Perm. Oil [.frac]
	float Cow;    // Water-oil Capillary pressure [psi]
};

struct Reservoir
{
	int nx, ny, nz; // grids
	float tx, ty, tz; // dimensions [ft]
	float pi, swi; // Initial pressure [psi], Initial water saturation [.frac]
	float phi0, crock, pref; // Porosity at reference [.frac], rock compressibility [1/psi], pressure reference [psi]
	float permx, permy, permz; // Permeability [mD]
};

struct PVTData
{
	Reservoir reservoir;
	std::vector<PVTO> oil;
	std::vector<PVTW> water;
	std::vector<PVTG> gas;
	std::vector<RockPhysic> rockPhysic;
};