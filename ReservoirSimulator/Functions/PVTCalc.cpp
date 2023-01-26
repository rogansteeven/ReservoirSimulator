#include "PVTCalc.hpp"
#include "Interpolate.hpp"
#include <stdexcept>
#include <cmath>

std::unique_ptr<PVT> PVTCalc::pvt = nullptr;
const float PVTCalc::eps = 0.001f;

void PVTCalc::Init(const std::string& filepath)
{
	pvt = PVT::Create(filepath);
}

float PVTCalc::Calc(Props props, float p)
{
	switch (props)
	{
	case Props::Rhoo:
		return CalcRhoo(p);
	case Props::Rhog:
		return CalcRhog(p);
	case Props::Rhow:
		return CalcRhow(p);
	case Props::Phi:
		return CalcPhi(p);
	}

	// Props that only need interpolation
	auto [ps, tab] = SelectProps(props);
	return Interpolate(ps, tab, p);
}

float PVTCalc::CalcDeriv(Props props, float p)
{
	return (Calc(props, p) - Calc(props, p - eps)) / eps;
}

float PVTCalc::CalcRhoo(float p)
{
	float Rhoos = pvt->GetData().density.oil;
	float Rhogs = pvt->GetData().density.gas;
	float Rs = Calc(Props::Rs, p);
	float Bo = Calc(Props::Bo, p);
	return (Rhoos + Rs * Rhogs) / Bo;
}

float PVTCalc::CalcRhog(float p)
{
	float Rhogs = pvt->GetData().density.gas;
	float Bg = Calc(Props::Bg, p);
	return Rhogs / Bg;
}

float PVTCalc::CalcRhow(float p)
{
	float Rhows = pvt->GetData().density.water;
	float Bw = Calc(Props::Bw, p);
	return Rhows / Bw;
}

float PVTCalc::CalcPhi(float p)
{
	float phi0 = pvt->GetData().reservoir.phi0;
	float cr = pvt->GetData().reservoir.crock;
	float p0 = pvt->GetData().reservoir.pref;
	return phi0 * exp(cr * (p - p0));
}

PVTCalc::Pair PVTCalc::SelectProps(Props props)
{
	std::vector<float> ps;
	std::vector<float> tabprop;

	switch (props)
	{
	case Props::Bo:
	{
		for (const auto& entry : pvt->GetData().pvto)
		{
			ps.push_back(entry.Po);
			tabprop.push_back(entry.Bo);
		}
		break;
	}
	case Props::Viso:
	{
		for (const auto& entry : pvt->GetData().pvto)
		{
			ps.push_back(entry.Po);
			tabprop.push_back(entry.Viso);
		}
		break;
	}
	case Props::Rs:
	{
		for (const auto& entry : pvt->GetData().pvto)
		{
			ps.push_back(entry.Po);
			tabprop.push_back(entry.Rs);
		}
		break;
	}
	case Props::Bw:
	{
		for (const auto& entry : pvt->GetData().pvtw)
		{
			ps.push_back(entry.Pref);
			tabprop.push_back(entry.Bwref);
		}
		break;
	}
	case Props::Visw:
	{
		for (const auto& entry : pvt->GetData().pvtw)
		{
			ps.push_back(entry.Pref);
			tabprop.push_back(entry.Visw);
		}
		break;
	}
	case Props::Bg:
	{
		for (const auto& entry : pvt->GetData().pvtg)
		{
			ps.push_back(entry.Pg);
			tabprop.push_back(entry.Bg);
		}
		break;
	}
	default:
		throw std::runtime_error("Please select appropiate props!");
	}
	return { ps, tabprop };
}
