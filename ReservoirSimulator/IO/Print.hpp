#pragma once

#include "../Functions/PVT.hpp"
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, const Data& data);

std::ostream& operator<<(std::ostream& os, const Reservoir& reservoir);
std::ostream& operator<<(std::ostream& os, const Density& density);

std::ostream& operator<<(std::ostream& os, const std::vector<PVTO>& pvto);
std::ostream& operator<<(std::ostream& os, const std::vector<PVTG>& pvtg);
std::ostream& operator<<(std::ostream& os, const std::vector<PVTW>& pvtw);

std::ostream& operator<<(std::ostream& os, const std::vector<RockPhysic>& rockphysic);