#include "Simulator\Data.hpp"
#include "IO\Print.hpp"

#include <iostream>

//#define PrintCalc(prop, x) std::cout << #prop << PVTCalc::Calc(Props::##prop, x) << '\n'

int main()
{
	auto data = Data::Create("Data\\project2.txt");
	//auto model = Model::Create(data);
	//Simulator::Init(data, model);

	std::cout << data << std::endl;
}