#pragma once
#include "../Data/Properties.hpp"
#include <vector>

class PVTCalc
{
public:
	static void Init(PVT* _pvt);

	static float Calc(Props props, float p);
	static float CalcDeriv(Props props, float p);

private:
	struct Pair
	{
		std::vector<float> xs;
		std::vector<float> ys;
	};
	static Pair SelectProps(Props props);

private:
	static PVT* pvt;
	static const float eps;
};