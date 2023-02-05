#include "Simulator.hpp"
#include "Block.hpp"
#include "../Functions/Interpolate.hpp"

#include <stdexcept>
#include <cmath>
#include <format>
#include <iostream>

std::shared_ptr<Data> Simulator::m_Data = nullptr;
std::shared_ptr<Model> Simulator::m_Model = nullptr;

void Simulator::Init(const std::shared_ptr<Data>& data, const std::shared_ptr<Model>& model)
{
	m_Data = data;
	m_Model = model;

	// Model Initialization
	CalcPressureDistribution();
	CalcOriginalInPlace();
	CalcPotentialFlowDistribution();
}

float Simulator::Calc(Props props, float x)
{
	float value;

	switch (props)
	{
	case Props::Rhoo:
		value = CalcOilDensity(x);          break;
	case Props::Rhog:
		value = CalcGasDensity(x);          break;
	case Props::Rhow:
		value = CalcWaterDensity(x);        break;
	case Props::Phi:
		value = CalcPorosity(x);            break;
	default:
	{
		auto [xs, ys] = SelectTable(props);
		value = Interpolate(xs, ys, x);
	}
	}

	return UnitConverter(props, value);
}

float Simulator::CalcDeriv(Props props, float x)
{	
	return (Calc(props, x) - Calc(props, x - Eps(props))) / Eps(props);
}

float Simulator::CalcRate(RateType rateType, int index, float time)
{
	switch (rateType)
	{
	case RateType::Oil:
		return CalcOilRate(index, time);
	case RateType::Water:
		return CalcWaterRate(index, time);
	default:
		throw std::runtime_error("Unknown rate type!");
	}
}

float Simulator::CalcRateDeriv(RateType rateType, Props props, int index, float time)
{
	// Calc
	switch (rateType)
	{
	case RateType::Oil:
		return (CalcOilRate(index, time) - CalcOilRate(index, time, props)) / Eps(props);
	case RateType::Water:
		return (CalcWaterRate(index, time) - CalcWaterRate(index, time, props)) / Eps(props);
	default:
		throw std::runtime_error("Unknown rate type!");
	}
}

bool Simulator::IsBlockPoten(int i, int j, int k, PotenDir potenDir)
{
	const auto& blocks = m_Model->GetBlocks();
	return blocks[i][j][k].poten.IsPoten(potenDir);
}

unsigned char Simulator::BlockPoten(int i, int j, int k)
{
	return m_Model->GetBlocks()[i][j][k].poten.poten;
}

float Simulator::GetBlockPressure(int i, int j, int k)
{
	const auto& block = m_Model->GetBlocks();
	return block[i][j][k].p;
}

void Simulator::SetBlockPressure(int i, int j, int k, float p)
{
	auto& blocks = m_Model->SetBlocks();
	blocks[i][j][k].p = p;
}

float Simulator::CalcOilRate(int index, float time, Props props)
{
	const Data::Content& content = m_Data->GetContent();
	if (content.wells[index].timeRate[0].rate > 0.0f) // If rate is positive, it is an injection well
		return 0.0f;

	return (1.0f - CalcFractionalW(index, props)) * CalcRate(index, time);
}

float Simulator::CalcWaterRate(int index, float time, Props props)
{
	const Data::Content& content = m_Data->GetContent();
	if (content.wells[index].timeRate[0].rate > 0.0f) // If rate is positive, it is an injection well
		return CalcRate(index, time);

	return CalcFractionalW(index, props) * CalcRate(index, time);
}

float Simulator::CalcOilDensity(float x)
{
	const Data::Content& content = m_Data->GetContent();
	float rhoo = content.density.oil;
	float rhog = content.density.gas;
	float rs = Calc(Props::Rs, x);
	float bo = Calc(Props::Bo, x);
	return (rhoo + rhog * rs) / bo;
}

float Simulator::CalcGasDensity(float x)
{
	const Data::Content& content = m_Data->GetContent();
	float rhog = content.density.gas;
	float bg = Calc(Props::Bg, x);
	return rhog / bg;
}

float Simulator::CalcWaterDensity(float x)
{
	const Data::Content& content = m_Data->GetContent();
	float rhow = content.density.water;
	float bw = Calc(Props::Bw, x);
	return rhow / bw;
}

float Simulator::CalcPorosity(float x)
{
	const Data::Content& content = m_Data->GetContent();
	float phi0 = content.reservoir.phi0;
	float cr = content.reservoir.crock;
	float p0 = content.reservoir.pref;
	return phi0 * exp(cr * (x - p0));
}

float Simulator::CalcRate(int index, float time)
{
	const Data::Content& content = m_Data->GetContent();

	if (index >= content.wells.size())
		throw std::runtime_error(std::format("Well index out of range. Please select between 0-{}!", content.wells.size()));

	for (const auto& [wellTime, wellRate] : content.wells[index].timeRate)
		if (time >= 0.0f && time <= wellTime)
			return wellRate;

	return 0.0f;
}

float Simulator::CalcFractionalW(int index, Props props)
{
	const Data::Content& content = m_Data->GetContent();

	if (index >= content.wells.size())
		throw std::runtime_error(std::format("Well index out of range. Please select between 0-{}!", content.wells.size()));

	int i = content.wells[index].i;
	int j = content.wells[index].j;
	int k = content.wells[index].k;

	float p = m_Model->GetBlocks()[i][j][k].p - Eps(props);
	float sw = m_Model->GetBlocks()[i][j][k].sw - Eps(props);

	float viso = Calc(Props::Viso, p);
	float visw = Calc(Props::Visw, p);
	float kro = Calc(Props::Kro, sw);
	float krw = Calc(Props::Krw, sw);

	float M = (visw / viso) * (kro / krw);
	return 1.0f / (1.0f + M);
}

void Simulator::CalcPressureDistribution()
{
	// Get BlockSizes and Blocks
	auto& [nx, ny, nz] = m_Model->GetBlockSize();
	const auto& blocks = m_Model->GetBlocks();
	auto& setBlocks = m_Model->SetBlocks();

	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = nz - 2; k >= 0; k--)
			{
				// Initalize data
				float p1 = blocks[i][j][k + 1].p;
				float p0 = blocks[i][j][k].p;
				float phalf = 0.5f * (p0 + p1);
				float dz = blocks[i][j][k].dz;
				float dno = Calc(Props::Rhoo, phalf) * dz;
				float diff = p1 - p0 - dno;

				// Loop until diff below epsilon
				while (abs(diff) > Eps())
				{
					p0 += diff * 0.5f;
					phalf = 0.5f * (p0 + p1);
					dno = Calc(Props::Rhoo, phalf) * dz;
					diff = p1 - p0 - dno;
				}

				setBlocks[i][j][k].p = p0;
			}
}

void Simulator::CalcOriginalInPlace()
{
	// Get BlockSizes and Blocks
	const auto& [nx, ny, nz] = m_Model->GetBlockSize();
	const auto& blocks = m_Model->GetBlocks();

	float ooip = 0.0f;
	float ogip = 0.0f;

	for (int i = 0; i < nx; i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz - 1; k++)
			{
				// Data
				float v   = blocks[i][j][k].Volume();
				float phi = blocks[i][j][k].Phi();

				float so  = blocks[i][j][k].So();
				float bo  = blocks[i][j][k].Bo();
						  
				float rs  = blocks[i][j][k].Rs();
				float sg  = blocks[i][j][k].sg;
				float bg  = blocks[i][j][k].Bg();

				// Calc
				ooip += v * phi * (so / bo);
				ogip += v * phi * (sg / bg + rs * so / bo);
			}

	m_Model->SetOOIP(ooip);
	m_Model->SetOGIP(ogip);
}

void Simulator::CalcPotentialFlowDistribution()
{
	const Data::Content& content = m_Data->GetContent();
	auto& blocks = m_Model->SetBlocks();

	int nx = content.reservoir.nx;
	int ny = content.reservoir.ny;
	int nz = content.reservoir.nz;

	for (int i = 0;i < nx;i++)
		for (int j = 0; j < ny; j++)
			for (int k = 0; k < nz; k++)
			{
				if (k > 1)
				{
					CalcPotentialFlow(blocks[i][j][k], blocks[i][j][k - 1], PotenDir::topW);
					CalcPotentialFlow(blocks[i][j][k], blocks[i][j][k - 1], PotenDir::topO);
				}

				if (k < nz - 1)
				{
					CalcPotentialFlow(blocks[i][j][k], blocks[i][j][k + 1], PotenDir::bottomW);
					CalcPotentialFlow(blocks[i][j][k], blocks[i][j][k + 1], PotenDir::bottomO);
				}

				if (j > 1)
					CalcPotentialFlow(blocks[i][j][k], blocks[i][j - 1][k], PotenDir::frontW);

				if (j < ny - 1)
					CalcPotentialFlow(blocks[i][j][k], blocks[i][j + 1][k], PotenDir::backW);

				if (i > 1)
					CalcPotentialFlow(blocks[i][j][k], blocks[i - 1][j][k], PotenDir::leftW);

				if (i < nx - 1)
					CalcPotentialFlow(blocks[i][j][k], blocks[i + 1][j][k], PotenDir::rightW);
			}
}

void Simulator::CalcPotentialFlow(Block& currBlock, Block& neighBlock, PotenDir potenDir)
{
	float pCurr = currBlock.p;
	float pNeigh = neighBlock.p;
	float pHalf = 0.5f * (pCurr + pNeigh);
	float dz = currBlock.dz;
	float poten = pNeigh - pCurr;
	
	switch (potenDir)
	{
	case PotenDir::topW:
		poten -= Calc(Props::Rhow, pHalf) * dz; break;
	case PotenDir::topO:
		poten -= Calc(Props::Rhoo, pHalf) * dz; break;
	case PotenDir::bottomW:
		poten += Calc(Props::Rhow, pHalf) * dz; break;
	case PotenDir::bottomO:
		poten += Calc(Props::Rhoo, pHalf) * dz; break;
	}

	if (poten > 0.0f)
		currBlock.poten.SetPoten(potenDir, true);
	else
		currBlock.poten.SetPoten(potenDir, false);
}

Simulator::SelectPairs Simulator::SelectTable(Props props)
{
	const Data::Content& content = m_Data->GetContent();
	std::vector<float> xs;
	std::vector<float> ys;

	switch (props)
	{
	case Props::Bo:
	{
		for (const auto& entry : content.pvto)
		{
			xs.push_back(entry.p);
			ys.push_back(entry.bo);
		}
		return { xs, ys };
	}
	case Props::Viso:
	{
		for (const auto& entry : content.pvto)
		{
			xs.push_back(entry.p);
			ys.push_back(entry.viso);
		}
		return { xs, ys };
	}
	case Props::Rs:
	{
		for (const auto& entry : content.pvto)
		{
			xs.push_back(entry.p);
			ys.push_back(entry.rs);
		}
		return { xs, ys };
	}
	case Props::Bw:
	{
		for (const auto& entry : content.pvtw)
		{
			xs.push_back(entry.pref);
			ys.push_back(entry.bwref);
		}
		return { xs, ys };
	}
	case Props::Visw:
	{
		for (const auto& entry : content.pvtw)
		{
			xs.push_back(entry.pref);
			ys.push_back(entry.viswref);
		}
		return { xs, ys };
	}
	case Props::Bg:
	{
		for (const auto& entry : content.pvtg)
		{
			xs.push_back(entry.p);
			ys.push_back(entry.bg);
		}
		return { xs, ys };
	}
	case Props::Visg:
	{
		for (const auto& entry : content.pvtg)
		{
			xs.push_back(entry.p);
			ys.push_back(entry.visg);
		}
		return { xs, ys };
	}
	case Props::Kro:
	{
		for (const auto& entry : content.rockPhysic)
		{
			xs.push_back(entry.sw);
			ys.push_back(entry.kro);
		}
		return { xs, ys };
	}
	case Props::Krw:
	{
		for (const auto& entry : content.rockPhysic)
		{
			xs.push_back(entry.sw);
			ys.push_back(entry.krw);
		}
		return { xs, ys };
	}
	case Props::Pcow:
	{
		for (const auto& entry : content.rockPhysic)
		{
			xs.push_back(entry.sw);
			ys.push_back(entry.pcow);
		}
		return { xs, ys };
	}
	default:
		throw std::runtime_error("Please select from available props.");
	}
}

float Simulator::UnitConverter(Props props, float value)
{
	switch (props)
	{
	case Props::Rs:
		return value / 5.6146f;
	case Props::Bg:
		return 5.6146f * value;
	case Props::Rhoo:
		return 6.95e-3f * value; // lb/cuft -> psi/ft
	case Props::Rhog:
		return 6.95e-3f * value; // lb/cuft -> psi/ft
	case Props::Rhow:
		return 6.95e-3f * value; // lb/cuft -> psi/ft
	default:
		return value;
	}
}

constexpr float Simulator::Eps(Props props)
{
	float eps = Eps();
	switch (props)
	{
	case Props::None:
		eps = 0.0f; break;

	case Props::Kro:
	case Props::Krw:
	case Props::Pcow:
	case Props::Sw:
		eps = Eps() * 0.1f; break;
	}
	return eps;
}
