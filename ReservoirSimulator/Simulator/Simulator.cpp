#include "Simulator.hpp"
#include "../Functions/Interpolate.hpp"
#include "Block.hpp"

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
		return OilRate(index, time);
	case RateType::Water:
		return WaterRate(index, time);
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
		return (OilRate(index, time) - OilRate(index, time, props)) / Eps(props);
	case RateType::Water:
		return (WaterRate(index, time) - WaterRate(index, time, props)) / Eps(props);
	default:
		throw std::runtime_error("Unknown rate type!");
	}
}

float Simulator::OilRate(int index, float time, Props props)
{
	const Data::Content& content = m_Data->GetContent();
	if (content.wells[index].timeRate[0].rate > 0.0f)
		return 0.0f;

	return (1.0f - FracionalW(index, props)) * Rate(index, time);
}

float Simulator::WaterRate(int index, float time, Props props)
{
	const Data::Content& content = m_Data->GetContent();
	if (content.wells[index].timeRate[0].rate > 0.0f)
		return Rate(index, time);

	return FracionalW(index, props) * Rate(index, time);
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

float Simulator::Rate(int index, float time)
{
	const Data::Content& content = m_Data->GetContent();

	if (index >= content.wells.size())
		throw std::runtime_error(std::format("Well index out of range. Please select between 0-{}!", content.wells.size()));

	for (const auto& [wellTime, wellRate] : content.wells[index].timeRate)
		if (time >= 0.0f && time <= wellTime)
			return wellRate;

	return 0.0f;
}

float Simulator::FracionalW(int index, Props props)
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
	// Get Coordinates and Blocks
	auto& [nx, ny, nz] = m_Model->GetCoordinate();
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
	// Get Coordinates and Blocks
	const auto& [nx, ny, nz] = m_Model->GetCoordinate();
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
		return 6.95e-3f * value;
	case Props::Rhog:
		return 6.95e-3f * value;
	case Props::Rhow:
		return 6.95e-3f * value;
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