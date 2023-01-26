#include "PVTReader.hpp"
#include "Reader.hpp"

PVT PVTReader(const std::string& filepath)
{
	Reader reader{ filepath };
	std::vector<PVTO> pvto;
	std::vector<PVTW> pvtw;

	// Number of entries
	int n = std::stoi(reader.ReadLine(4));

	// Read PVT Oil
	reader.ReadLine(4, false); // Skip to the first entry of PVT Oil

	// Read PVT Water
	reader.ReadLine(5, false); // Skip to the first entry of PVT Water

	float Pref, Bwref, Cwref, Visw, Visbw;
	reader.GetFile() >> Pref >> Bwref >> Cwref >> Visw >> Visbw;
	pvtw.push_back({ Pref, Bwref, Cwref, Visw, Visbw });

	return { pvto, pvtw };
}

std::vector<PVTO> PVTOReader(int n)
{
	float Rs, Po, Bo, Viso;
	for (int i = 0; i < n; i++)
	{
		reader.GetFile() >> Rs >> Po >> Bo >> Viso;
		pvto.push_back({ Rs, Po, Bo, Viso });
	}
}
