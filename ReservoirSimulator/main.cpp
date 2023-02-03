#include "Simulator\Data.hpp"
#include "Simulator\Model.hpp"
#include "Simulator\Simulator.hpp"
#include "Simulator\Typedef.hpp"
#include "IO\Print.hpp"
#include "Simulator\Block.hpp"

#include <iostream>

void TestData(const std::shared_ptr<Data>& data);
void TestModel(const std::shared_ptr<Model>& model);
void TestRate();
void TestCalc();

int main()
{
	DataSourceMap dataSources = {
		{DataSource::P2, "Data\\project2.txt"},
		{DataSource::P4, "Data\\well_data.txt"}
	};

	auto data = Data::Create(dataSources);
	auto model = Model::Create(data);
	Simulator::Init(data, model);

	
	//TestData(data);
	TestRate();
}

void TestData(const std::shared_ptr<Data>& data)
{
	std::cout << data << std::endl;
}

void TestModel(const std::shared_ptr<Model>& model)
{
	std::cout << model << std::endl;
	std::cout << "P[0]: " << model->GetBlocks()[0][0][0].p << std::endl;
	std::cout << "P[1]: " << model->GetBlocks()[0][0][1].p << std::endl;
	std::cout << "P[2]: " << model->GetBlocks()[0][0][2].p << std::endl;
	std::cout << "P[3]: " << model->GetBlocks()[0][0][3].p << std::endl;
	std::cout << "P[4]: " << model->GetBlocks()[0][0][4].p << std::endl;
}

void TestRate()
{
	std::cout << "Oil Rate Production [1600]: " << abs(Simulator::CalcRate(RateType::Oil, 1, 1600)) << std::endl;
	std::cout << "Oil Rate Derivative [1600]: " << abs(Simulator::CalcRateDeriv(RateType::Oil, Props::Sw, 1, 1600)) << std::endl;

	std::cout << "Water Rate Production [1600]: " << abs(Simulator::CalcRate(RateType::Water, 1, 1600)) << std::endl;
	std::cout << "Water Rate Derivative [1600]: " << abs(Simulator::CalcRateDeriv(RateType::Water, Props::Sw, 1, 1600)) << std::endl;

	std::cout << "Water Rate Injection [1600]: " << abs(Simulator::CalcRate(RateType::Water, 0, 1600)) << std::endl;
}

void TestCalc()
{
	std::cout << "Calc Deriv: " << Simulator::CalcDeriv(Props::Bo, 1300.0f) << std::endl;
}