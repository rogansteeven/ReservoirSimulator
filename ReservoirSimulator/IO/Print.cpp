#include "Print.hpp"

std::ostream& operator<<(std::ostream& os, std::vector<PVTO> pvto)
{
	os << "Rs" << " " << "Po" << " " << "Bo" << " " << "Viso\n";
	for (const auto& entry : pvto)
		os << entry.Rs << " " << entry.Po << " " << entry.Bo << " " << entry.Viso << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, std::vector<PVTW> pvtw)
{
	os << "Pref" << " " << "Bwref" << " " << "Cwref" << " " << "Viscosity" << " " << "Viscobility\n";
	for (const auto& entry : pvtw)
		os << entry.Pref << " " << entry.Bwref << " " << entry.Cwref << " " << entry.Visw << " " << entry.Visbw << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, PVT pvt)
{
	return os << pvt.oil << '\n' << pvt.water;
}
