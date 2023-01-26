#include "IO\PVTReader.hpp"
#include "IO\Print.hpp"

int main()
{
	PVT pvt = PVTReader("Data\\pvt.txt");
	std::cout << pvt << std::endl;
}