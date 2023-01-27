#include "Print.hpp"

std::ostream& operator<<(std::ostream& os, const Data& data)
{
	os << data.reservoir << '\n'
	   << data.rockPhysic << '\n'
	   << data.density << '\n'
	   << data.pvto << '\n'
	   << data.pvtg << '\n'
	   << data.pvtw << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const Reservoir& reservoir)
{
	// Print grids
	os << "nx" << " " << "ny" << " " << "nz\n";
	os << reservoir.nx << " " << reservoir.ny << " " << reservoir.nz << "\n\n";

	// Print dimensions
	os << "tx" << " " << "ty" << " " << "tz\n";
	os << reservoir.tx << " " << reservoir.ty << " " << reservoir.tz << "\n\n";

	// Print pressure initial and water saturation initial
	os << "Pi" << " " << "Swi\n";
	os << reservoir.pi << " " << reservoir.swi << "\n\n";

	// Print porosity, compressibility, and pressure reference
	os << "Phi0" << " " << "Crock" << " " << "Pref\n";
	os << reservoir.phi0 << " " << reservoir.crock << " " << reservoir.pref << "\n\n";

	// Print permeability
	os << "permx" << " " << "permy" << " " << "permz\n";
	os << reservoir.permx << " " << reservoir.permy << " " << reservoir.permz << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<RockPhysic>& rockPhysic)
{
	os << "Sw" << " " << "Krw" << " " << "Kro" << " " << "Pcow\n";
	for (const auto& entry : rockPhysic)
		os << entry.Sw << " " << entry.Krw << " " << entry.Kro << " " << entry.Pcow << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const Density& density)
{
	os << "Rho_o" << " " << "Rho_g" << " " << "Rho_w\n";
	os << density.oil << " " << density.gas << " " << density.water << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PVTO>& pvto)
{
	os << "Rs" << " " << "Po" << " " << "Bo" << " " << "Viso\n";
	for (const auto& entry : pvto)
		os << entry.Rs << " " << entry.Po << " " << entry.Bo << " " << entry.Viso << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PVTG>& pvtg)
{
	os << "Pg" << " " << "Bg" << " " << "Vg\n";
	for (const auto& entry : pvtg)
		os << entry.Pg << " " << entry.Bg << " " << entry.Vg << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PVTW>& pvtw)
{
	os << "Pref" << " " << "Bwref" << " " << "Cwref" << " " << "Viscosity" << " " << "Viscobility\n";
	for (const auto& entry : pvtw)
		os << entry.Pref << " " << entry.Bwref << " " << entry.Cwref << " " << entry.Visw << " " << entry.Visbw << '\n';
	return os;
}
