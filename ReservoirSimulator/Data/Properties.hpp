#pragma once
#include <vector>

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