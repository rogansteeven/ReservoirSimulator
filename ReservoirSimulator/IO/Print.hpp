#pragma once

#include "../Data/Properties.hpp"
#include <iostream>
#include <vector>

std::ostream& operator<<(std::ostream& os, std::vector<PVTO> pvto);
std::ostream& operator<<(std::ostream& os, std::vector<PVTW> pvtw);
std::ostream& operator<<(std::ostream& os, PVT pvt);