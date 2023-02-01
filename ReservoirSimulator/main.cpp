#include "Simulator\Data.hpp"
#include "Simulator\Model.hpp"
#include "Simulator\Simulator.hpp"
#include "IO\Print.hpp"
#include "Simulator\Block.hpp"

#include <iostream>

//#define PrintCalc(prop, x) std::cout << #prop << PVTCalc::Calc(Props::##prop, x) << '\n'

int main()
{
	auto data = Data::Create("Data\\project2.txt");
	auto model = Model::Create(data);
	Simulator::Init(data, model);

	std::cout << model << std::endl;
	std::cout << "P[0]: " << model->GetBlocks()[0][0][0].p << std::endl;
	std::cout << "P[1]: " << model->GetBlocks()[0][0][1].p << std::endl;
	std::cout << "P[2]: " << model->GetBlocks()[0][0][2].p << std::endl;
	std::cout << "P[3]: " << model->GetBlocks()[0][0][3].p << std::endl;
	std::cout << "P[4]: " << model->GetBlocks()[0][0][4].p << std::endl;
	//std::cout << Simulator::Calc(Props::Rs, 6000.0f) << std::endl;
}