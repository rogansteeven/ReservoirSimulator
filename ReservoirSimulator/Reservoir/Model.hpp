#pragma once

#include "../Data/Properties.hpp"
#include "Block.hpp"

class Model
{
public:
	using Grid  = std::vector<Block>;
	using Grid2 = std::vector<Grid>;
	using Grid3 = std::vector<Grid2>;

	static void Init(const Data& data);

	static void CalcPressureDistribution();
	static float CalcPressure(float p0, float p1);

	static const Grid3 GetGrid() { return m_Grid; }

private:
	static int nx, ny, nz;
	static Grid3 m_Grid;
	static const float eps;
};