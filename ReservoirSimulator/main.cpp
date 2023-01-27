#include "Functions\PVT.hpp"
#include "IO\Print.hpp"
#include "Functions\PVTCalc.hpp"

int main()
{
	PVTCalc::Init("Data\\project2.txt");

	std::cout << PVTCalc::GetData() << std::endl;
	std::cout << "Bo: " << PVTCalc::CalcDeriv(Props::Kro, 0.3f) << '\n';
}