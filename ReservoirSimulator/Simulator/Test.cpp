#include "Test.hpp"

#include "Block.hpp"
#include "Simulator.hpp"
#include "../IO/Print.hpp"

#include <bitset>

namespace Test
{
	void Data1(const std::shared_ptr<Data>& data)
	{
		std::cout << data << std::endl;
	}

	void Model1(const std::shared_ptr<Model>& model)
	{
		std::cout << model << std::endl;
		std::cout << "P[0]: " << model->GetBlocks()[0][0][0].p << std::endl;
		std::cout << "P[1]: " << model->GetBlocks()[0][0][1].p << std::endl;
		std::cout << "P[2]: " << model->GetBlocks()[0][0][2].p << std::endl;
		std::cout << "P[3]: " << model->GetBlocks()[0][0][3].p << std::endl;
		std::cout << "P[4]: " << model->GetBlocks()[0][0][4].p << std::endl;
	}

	void Rate1()
	{
		std::cout << "Oil Rate Production [1600]: " << abs(Simulator::CalcRate(RateType::Oil, 1, 1600)) << std::endl;
		std::cout << "Oil Rate Derivative [1600]: " << abs(Simulator::CalcRateDeriv(RateType::Oil, Props::Sw, 1, 1600)) << std::endl;

		std::cout << "Water Rate Production [1600]: " << abs(Simulator::CalcRate(RateType::Water, 1, 1600)) << std::endl;
		std::cout << "Water Rate Derivative [1600]: " << abs(Simulator::CalcRateDeriv(RateType::Water, Props::Sw, 1, 1600)) << std::endl;

		std::cout << "Water Rate Injection [1600]: " << abs(Simulator::CalcRate(RateType::Water, 0, 1600)) << std::endl;
	}

	void Calc1()
	{
		std::cout << "Calc Bo [1300]: " << Simulator::Calc(Props::Bo, 1300) << std::endl;
		std::cout << "Calc dBo [1300]: " << Simulator::CalcDeriv(Props::Bo, 1300) << std::endl;

		std::cout << "Calc Rs [1300]: " << Simulator::Calc(Props::Rs, 1300) << std::endl;
		std::cout << "Calc dRs [1300]: " << Simulator::CalcDeriv(Props::Rs, 1300) << std::endl;

		std::cout << "Calc Rhoo [2080]: " << Simulator::Calc(Props::Rhoo, 2080) << std::endl;
		std::cout << "Calc dRhoo [2080]: " << Simulator::CalcDeriv(Props::Rhoo, 2080) << std::endl;

		std::cout << "Calc Kro [0.85]: " << Simulator::Calc(Props::Kro, 0.85f) << std::endl;
		std::cout << "Calc dKro [0.85]: " << Simulator::CalcDeriv(Props::Kro, 0.85f) << std::endl;

		std::cout << "Calc Krw [0.85]: " << Simulator::Calc(Props::Krw, 0.85f) << std::endl;
		std::cout << "Calc dKrw [0.85]: " << Simulator::CalcDeriv(Props::Krw, 0.85f) << std::endl;
	}

	void Poten1()
	{
		std::cout << "Has Block[1][2][3] a potential of oil flow to bottom? " << Simulator::IsBlockPoten(1, 2, 3, PotenDir::bottomO) << std::endl;
		std::cout << "Block[1][2][3] potential: " << std::bitset<8>(Simulator::BlockPoten(1, 2, 3)) << std::endl;
	}
}