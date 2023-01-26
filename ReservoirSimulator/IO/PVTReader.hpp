#pragma once

#include <string>
#include "../Data/Properties.hpp"

PVT PVTReader(const std::string& filepath);
std::vector<PVTO> PVTOReader(int n);
std::vector<PVTW> PVTOReader(int n);