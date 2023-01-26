#include "Functions\PVT.hpp"
#include "IO\Print.hpp"
#include "Functions\PVTCalc.hpp"

int main()
{
	PVTCalc::Init("Data\\project2.txt");

	//std::cout << PVTCalc::GetData() << std::endl;
	std::cout << PVTCalc::Calc(Props::Phi, 1500.0f) << std::endl;
}