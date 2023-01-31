#include "Simulator.hpp"
#include "../Functions/Interpolate.hpp"

#include <stdexcept>
#include <cmath>

std::shared_ptr<Data> Simulator::m_Data = nullptr;
std::shared_ptr<Data> Simulator::m_Model = nullptr;
float Simulator::m_Eps = 0.001f;

void Simulator::Init(const std::shared_ptr<Data>& data)
{
	m_Data = data;
}

float Simulator::Calc(Props props, float x)
{
	// Specific Calculation
	switch (props)
	{
	case Props::Rhoo:
		return CalcOilDensity(x);
	case Props::Rhog:
		return CalcGasDensity(x);
	case Props::Rhow:
		return CalcWaterDensity(x);
	case Props::Phi:
		return CalcPorosity(x);
	}

	// Interpolation
	auto [xs, ys] = SelectTable(props);
	float value = Interpolate(xs, ys, x);
	return UnitConverter(props, value);
}

float Simulator::CalcDeriv(Props props, float x)
{
	return (Calc(props, x) - Calc(props, x - m_Eps)) / m_Eps;
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
