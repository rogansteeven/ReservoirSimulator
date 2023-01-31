#pragma once

#include "../Simulator/Data.hpp"
#include "../Simulator/DataType.hpp"

#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Data>& data);

std::ostream& operator<<(std::ostream& os, const ReservoirData& reservoir);
std::ostream& operator<<(std::ostream& os, const DensityData& density);

std::ostream& operator<<(std::ostream& os, const std::vector<PVTOData>& pvto);
std::ostream& operator<<(std::ostream& os, const std::vector<PVTGData>& pvtg);
std::ostream& operator<<(std::ostream& os, const std::vector<PVTWData>& pvtw);

std::ostream& operator<<(std::ostream& os, const std::vector<RockPhysicData>& rockphysic);