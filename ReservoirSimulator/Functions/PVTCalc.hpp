#pragma once
#include "PVT.hpp"
#include <vector>

class PVTCalc
{
public:
	static void Init(const std::string& filepath);

	static float Calc(Props props, float p);
	static float CalcDeriv(Props props, float p);

	static float CalcRhoo(float p);
	static float CalcRhog(float p);
	static float CalcRhow(float p);
	static float CalcPhi(float p);

	static const Data GetData() { return pvt->GetData(); }

private:
	struct Pair
	{
		std::vector<float> xs;
		std::vector<float> ys;
	};
	static Pair SelectProps(Props props);

private:
	static std::unique_ptr<PVT> pvt;
	static const float eps;
};