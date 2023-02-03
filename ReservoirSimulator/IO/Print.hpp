#pragma once

#include "../Simulator/DataType.hpp"
#include "../Simulator/Data.hpp"
#include "../Simulator/Model.hpp"

#include <iostream>
#include <vector>

// DATA
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Data>& data);

std::ostream& operator<<(std::ostream& os, const ReservoirData& reservoir);
std::ostream& operator<<(std::ostream& os, const DensityData& density);

std::ostream& operator<<(std::ostream& os, const std::vector<PVTOData>& pvto);
std::ostream& operator<<(std::ostream& os, const std::vector<PVTGData>& pvtg);
std::ostream& operator<<(std::ostream& os, const std::vector<PVTWData>& pvtw);

std::ostream& operator<<(std::ostream& os, const std::vector<RockPhysicData>& rockphysic);

std::ostream& operator<<(std::ostream& os, const std::vector<WellData>& well);

// MODEL
std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Model>& model);