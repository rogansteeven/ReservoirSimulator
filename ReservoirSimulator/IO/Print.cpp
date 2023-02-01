#include "Print.hpp"

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Data>& data)
{
	os << data->GetContent().reservoir << '\n'
	   << data->GetContent().rockPhysic << '\n'
	   << data->GetContent().density << '\n'
	   << data->GetContent().pvto << '\n'
	   << data->GetContent().pvtg << '\n'
	   << data->GetContent().pvtw << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const ReservoirData& reservoir)
{
	// Print grids
	os << "nx" << " " << "ny" << " " << "nz\n";
	os << reservoir.nx << " " << reservoir.ny << " " << reservoir.nz << "\n\n";

	// Print dimensions
	os << "tx" << " " << "ty" << " " << "tz\n";
	os << reservoir.tx << " " << reservoir.ty << " " << reservoir.tz << "\n\n";

	// Print pressure initial and water saturation initial
	os << "Pi" << " " << "Swi\n";
	os << reservoir.pinit << " " << reservoir.swi << "\n\n";

	// Print porosity, compressibility, and pressure reference
	os << "Phi0" << " " << "Crock" << " " << "Pref\n";
	os << reservoir.phi0 << " " << reservoir.crock << " " << reservoir.pref << "\n\n";

	// Print permeability
	os << "permx" << " " << "permy" << " " << "permz\n";
	os << reservoir.permx << " " << reservoir.permy << " " << reservoir.permz << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<RockPhysicData>& rockPhysic)
{
	os << "Sw" << " " << "Krw" << " " << "Kro" << " " << "Pcow\n";
	for (const auto& entry : rockPhysic)
		os << entry.sw << " " << entry.krw << " " << entry.kro << " " << entry.pcow << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const DensityData& density)
{
	os << "Rho Oil" << " " << "Rho Gas" << " " << "Rho Water\n";
	os << density.oil << " " << density.gas << " " << density.water << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PVTOData>& pvto)
{
	os << "Rs" << " " << "Po" << " " << "Bo" << " " << "Viso\n";
	for (const auto& entry : pvto)
		os << entry.rs << " " << entry.p << " " << entry.bo << " " << entry.viso << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PVTGData>& pvtg)
{
	os << "Pg" << " " << "Bg" << " " << "Vg\n";
	for (const auto& entry : pvtg)
		os << entry.p << " " << entry.bg << " " << entry.visg << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<PVTWData>& pvtw)
{
	os << "Pref" << " " << "Bwref" << " " << "Cwref" << " " << "Viscosity" << " " << "Viscobility\n";
	for (const auto& entry : pvtw)
		os << entry.pref << " " << entry.bwref << " " << entry.cwref << " " << entry.viswref << " " << entry.viscosibility << '\n';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Model>& model)
{
	// Print grids
	const auto& [nx, ny, nz] = model->GetCoordinate();
	os << "nx" << " " << "ny" << " " << "nz\n";
	os << nx << " " << ny << " " << nz << "\n\n";

	// Print dimension
	const auto& [tx, ty, tz] = model->GetDimension();
	os << "tx" << " " << "ty" << " " << "tz\n";
	os << tx << " " << ty << " " << tz << "\n\n";

	// Print OOIP and OGIP
	const auto& ooip = model->GetOOIP();
	const auto& ogip = model->GetOGIP();
	os << "OOIP: " << ooip << '\n';
	os << "OGIP: " << ogip << "\n\n";

	return os;
}