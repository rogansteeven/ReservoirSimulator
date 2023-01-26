#include "IO\PVTReader.hpp"
#include "IO\Print.hpp"
#include "Functions\PVTCalc.hpp"

int main()
{
	PVT pvt = PVTReader("Data\\pvt.txt");
	PVTCalc::Init(&pvt);

	std::cout << pvt << std::endl;
	std::cout << PVTCalc::Calc(Props::Visw, 1500.0f) << std::endl;
}