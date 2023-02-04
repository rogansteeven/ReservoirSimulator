#pragma once

#include "Enum.hpp"
#include "Data.hpp"
#include "Model.hpp"

class Simulator
{
public:
	static void Init(const std::shared_ptr<Data>& data, const std::shared_ptr<Model>& model);

	static float Calc(Props props, float x);
	static float CalcDeriv(Props props, float x);

	static float CalcRate(RateType rateType, int index, float time);
	static float CalcRateDeriv(RateType rateType, Props props, int index, float time);

private:
	static float CalcOilDensity(float x);
	static float CalcGasDensity(float x);
	static float CalcWaterDensity(float x);
	static float CalcPorosity(float x);

	static float OilRate(int index, float time, Props props = Props::None);
	static float WaterRate(int index, float time, Props props = Props::None);

	static float Rate(int index, float time);
	static float FracionalW(int index, Props props);

private:
	static void CalcPressureDistribution();
	static void CalcOriginalInPlace();

private:
	using SelectPairs = std::pair< std::vector<float>, std::vector<float> >;
	static SelectPairs SelectTable(Props props);
	static float UnitConverter(Props props, float value);

	static constexpr float Eps() { return 0.001f; }
	static constexpr float Eps(Props props);

private:
	static std::shared_ptr<Data> m_Data;
	static std::shared_ptr<Model> m_Model;
};