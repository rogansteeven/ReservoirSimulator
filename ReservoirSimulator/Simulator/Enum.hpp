#pragma once

enum class Props
{
	None = 0,
	Bo, Viso, Rs, 
	Bw, Visw,
	Bg, Visg,
	Rhoo, Rhog, Rhow,
	Phi,
	Kro, Krw, Pcow,
	P, Sw
};

enum class DataSource
{
	P2 = 0, P4
};

enum class RateType
{
	Oil = 0, Water
};