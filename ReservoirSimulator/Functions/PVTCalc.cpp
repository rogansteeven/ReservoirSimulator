#include "PVTCalc.hpp"
#include "Interpolate.hpp"
#include <stdexcept>

PVT* PVTCalc::pvt = nullptr;
const float PVTCalc::eps = 0.001f;

void PVTCalc::Init(PVT* _pvt)
{
	pvt = _pvt;
}

float PVTCalc::Calc(Props props, float p)
{
	auto [ps, tab] = SelectProps(props);
	return Interpolate(ps, tab, p);
}

float PVTCalc::CalcDeriv(Props props, float p)
{
	return (Calc(props, p) - Calc(props, p - eps)) / eps;
}

PVTCalc::Pair PVTCalc::SelectProps(Props props)
{
	std::vector<float> ps;
	std::vector<float> tabprop;

	switch (props)
	{
	case Bo:
	{
		for (const auto& entry : pvt->oil)
		{
			ps.push_back(entry.Po);
			tabprop.push_back(entry.Bo);
		}
		break;
	}
	case Viso:
	{
		for (const auto& entry : pvt->oil)
		{
			ps.push_back(entry.Po);
			tabprop.push_back(entry.Viso);
		}
		break;
	}
	case Rs:
	{
		for (const auto& entry : pvt->oil)
		{
			ps.push_back(entry.Po);
			tabprop.push_back(entry.Rs);
		}
		break;
	}
	case Bw:
	{
		for (const auto& entry : pvt->water)
		{
			ps.push_back(entry.Pref);
			tabprop.push_back(entry.Bwref);
		}
		break;
	}
	case Visw:
	{
		for (const auto& entry : pvt->water)
		{
			ps.push_back(entry.Pref);
			tabprop.push_back(entry.Visw);
		}
		break;
	}
	default:
		throw std::runtime_error("Please select appropiate props!");
	}
	return { ps, tabprop };
}
