#pragma once

struct ReservoirData
{
	int nx, ny, nz;   // no of grids in x- y- and z-direction
	float tx, ty, tz; // dimension in x- y- and z-direction (ft)
	
	float pinit; // initial pressure at the center of top layer (psi)
	float swi;   // initial water saturation (.frac)

	float phi0;  // porosity at reference pressure (.frac)
	float crock; // rock compressibility (1/psi)
	float pref;  // reference pressure (psi)

	float permx, permy, permz; // permeability in x- y- and z-direction (md)
};

struct RockPhysicData
{
	float sw;   // Water saturation (.frac)
	float krw;  // rel.permeability to water (.frac)
	float kro;  // rel.permeability to oil (.frac)
	float pcow; // Water-oil Capillary pressure (psi)
};

struct DensityData
{
	float oil, gas, water; // oil, gas, and water densities at standard cond. (lb/cuft)
};

struct PVTOData
{
	float rs;   // [Mscf/stb]
	float p;    // [psia]
	float bo;   // [rb/stb]
	float viso; // [cp]
};

struct PVTGData
{
	float p;  // [psia]
	float bg; // [rb/mscf]
	float visg; // [cp]
};

struct PVTWData
{
	float pref;          // [psia]
	float bwref;         // [rb/stb]
	float cwref;         // [1/psia]
	float viswref;       // [cp]
	float viscosibility; // [1/psia]
};