#include "Model.hpp"
#include <stdexcept>
#include "../Functions/PVTCalc.hpp"

Model::Grid3 Model::m_Grid;
int Model::nx, Model::ny, Model::nz;
const float Model::eps = 0.001f;

void Model::Init(const Data& data)
{
	nx = data.reservoir.nx;
	ny = data.reservoir.ny;
	nz = data.reservoir.nz;

	float dx = data.reservoir.tx / nx;
	float dy = data.reservoir.ty / ny;
	float dz = data.reservoir.tz / nz;

	// Create all block grids
	m_Grid = Grid3(nx, Grid2(ny, Grid());
	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
				m_Grid[i][j].push_back(Block(dx, dy, dz, data.reservoir.pi, data.reservoir.swi));

	CalcPressureDistribution();
}

void Model::CalcPressureDistribution()
{
	float p0, p1, newP;
	float dz = m_Grid[0][0][0].dz;
	for (int k = 0; k < nz - 1; k++)
	{
		// Calculate new pressure
		p0 = m_Grid[0][0][k]; // Check first block pressure
		p1 = m_Grid[0][0][k + 1]; // Check second block pressure
		newP = CalcPressure(p0, p1);

		// Fill new pressure
		for (int i = 0; i < nx; i++)
			for (int j = 0; j < ny; j++)
				m_Grid[i][j][k + 1].p = newP;
	}
}

float Model::CalcPressure(float p0, float p1)
{
	float dp   = p0 - p1;
	float rho  = PVTCalc::Calc(Props::Rhoo, 0.5 * (p0 + p1));
	float flow = dp - rho * dz;

	while (abs(flow) > eps)
	{
		p1  += flow;
		dp   = p0 - p1;
		rho  = PVTCalc::Calc(Props::Rhoo, 0.5 * (p0 + p1));
		flow = dp - rho * dz;
	}
	return p1;
}