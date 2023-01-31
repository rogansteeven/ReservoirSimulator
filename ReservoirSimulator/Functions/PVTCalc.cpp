#include "PVTCalc.hpp"
#include "Interpolate.hpp"
#include <stdexcept>
#include <cmath>
#include "../Reservoir/Model.hpp"

std::unique_ptr<PVT> PVTCalc::pvt = nullptr;
const float PVTCalc::eps = 0.001f;

void PVTCalc::Init(const std::string& filepath)
{
	pvt = PVT::Create(filepath);
	Model::Init(pvt->GetData());
}

float PVTCalc::Calc(Props props, float x)
{
	switch (props)
	{
	case Props::Rhoo:
		return CalcRhoo(x);
	case Props::Rhog:
		return CalcRhog(x);
	case Props::Rhow:
		return CalcRhow(x);
	case Props::Phi:
		return CalcPhi(x);
	}

	// Props that only need interpolation
	auto [xs, tab] = SelectProps(props);
	auto result = Interpolate(xs, tab, x);
	return UnitConversion(props, result);
}

float PVTCalc::CalcDeriv(Props props, float x)
{
	return (Calc(props, x) - Calc(props, x - eps)) / eps;
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

float PVTCalc::UnitConversion(Props props, float result)
{
	switch (props)
	{
	case Props::Bg:
		return 5.6146f * result;
	case Props::Rs:
		return result / 5.6146f;
	case Props::Rhoo:
		return 6.95e-3f * result;
	case Props::Rhog:
		return 6.95e-3f * result;
	case Props::Rhow:
		return 6.95e-3f * result;
	default:
		return result;
	}
}

PVTCalc::Pair PVTCalc::SelectProps(Props props)
{
	std::vector<float> xs;
	std::vector<float> ys;

	switch (props)
	{
	case Props::Bo:
	{
		for (const auto& entry : pvt->GetData().pvto)
		{
			xs.push_back(entry.Po);
			ys.push_back(entry.Bo);
		}
		break;
	}
	case Props::Viso:
	{
		for (const auto& entry : pvt->GetData().pvto)
		{
			xs.push_back(entry.Po);
			ys.push_back(entry.Viso);
		}
		break;
	}
	case Props::Rs:
	{
		for (const auto& entry : pvt->GetData().pvto)
		{
			xs.push_back(entry.Po);
			ys.push_back(entry.Rs);
		}
		break;
	}
	case Props::Bw:
	{
		for (const auto& entry : pvt->GetData().pvtw)
		{
			xs.push_back(entry.Pref);
			ys.push_back(entry.Bwref);
		}
		break;
	}
	case Props::Visw:
	{
		for (const auto& entry : pvt->GetData().pvtw)
		{
			xs.push_back(entry.Pref);
			ys.push_back(entry.Visw);
		}
		break;
	}
	case Props::Bg:
	{
		for (const auto& entry : pvt->GetData().pvtg)
		{
			xs.push_back(entry.Pg);
			ys.push_back(entry.Bg);
		}
		break;
	}
	case Props::Kro:
	{
		for (const auto& entry : pvt->GetData().rockPhysic)
		{
			xs.push_back(entry.Sw);
			ys.push_back(entry.Kro);
		}
		break;
	}
	case Props::Krw:
	{
		for (const auto& entry : pvt->GetData().rockPhysic)
		{
			xs.push_back(entry.Sw);
			ys.push_back(entry.Krw);
		}
		break;
	}
	case Props::Pcow:
	{
		for (const auto& entry : pvt->GetData().rockPhysic)
		{
			xs.push_back(entry.Sw);
			ys.push_back(entry.Pcow);
		}
		break;
	}
	default:
		throw std::runtime_error("Please select appropiate props!");
	}
	return { xs, ys };
}
