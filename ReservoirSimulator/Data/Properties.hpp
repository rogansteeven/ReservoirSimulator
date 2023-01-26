#pragma once
#include <vector>

enum Props
{
	Bo = 0, Viso, Rs, Bw, Visw
};

struct PVTO
{
	float Rs, Po, Bo, Viso;
};

struct PVTW
{
	float Pref, Bwref, Cwref, Visw, Visbw;
};

struct PVT
{
	std::vector<PVTO> oil;
	std::vector<PVTW> water;
};