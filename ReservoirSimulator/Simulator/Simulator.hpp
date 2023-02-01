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

private:
	static float CalcOilDensity(float x);
	static float CalcGasDensity(float x);
	static float CalcWaterDensity(float x);
	static float CalcPorosity(float x);

private:
	static void CalcPressureDistribution();
	static void CalcOriginalInPlace();

private:
	using SelectPairs = std::pair< std::vector<float>, std::vector<float> >;
	static SelectPairs SelectTable(Props props);
	static float UnitConverter(Props props, float value);

private:
	static std::shared_ptr<Data> m_Data;
	static std::shared_ptr<Model> m_Model;
	static float m_Eps;
};