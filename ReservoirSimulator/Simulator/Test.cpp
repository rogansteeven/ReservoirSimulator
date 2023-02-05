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

	void Project5Problem()
	{
		const auto& [nx, ny, nz] = Simulator::GetBlockSize();
		float p;

#define INTMOD2(x) (int)((x+1) % 2 == 0)

		for (int i = 0;i < nx;i++)
			for (int j = 0; j < ny; j++)
				for (int k = 0; k < nz; k++)
				{
					if (int c; c = INTMOD2(i) + INTMOD2(j) + INTMOD2(k))
					{
						p = Simulator::GetBlockPressure(i, j, k);
						Simulator::SetBlockPressure(i, j, k, p + c * 10);
					}
				}

		Simulator::CalcPotentialFlowDistribution();

		int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
		for (int i = 0; i < nx; i++)
			for (int j = 0; j < ny; j++)
				for (int k = 0; k < nz; k++)
				{
					n1 += (int)Simulator::IsBlockPoten(i, j, k, PotenDir::leftW)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::rightW)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::frontW)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::backW);

					n2 += (int)Simulator::IsBlockPoten(i, j, k, PotenDir::topW)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::topO)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::bottomW)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::bottomO);

					n3 += (int)Simulator::IsBlockPoten(i, j, k, PotenDir::leftW)
						* (int)Simulator::IsBlockPoten(i, j, k, PotenDir::topW)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::rightW)
						* (int)Simulator::IsBlockPoten(i, j, k, PotenDir::bottomW);

					n4 += (int)Simulator::IsBlockPoten(i, j, k, PotenDir::backW)
						* (int)Simulator::IsBlockPoten(i, j, k, PotenDir::bottomO)
						+ (int)Simulator::IsBlockPoten(i, j, k, PotenDir::frontW)
						* (int)Simulator::IsBlockPoten(i, j, k, PotenDir::topO);
				}

		std::cout << "n1 = " << n1 << '\n';
		std::cout << "n2 = " << n2 << '\n';
		std::cout << "n3 = " << n3 << '\n';
		std::cout << "n4 = " << n4 << "\n\n";

		std::cout << "ibw(1, 2, 3) = " << Simulator::IsBlockPoten(1 - 1, 2 - 1, 3 - 1, PotenDir::leftW) << '\n';
		std::cout << "icw(2, 3, 4) = " << Simulator::IsBlockPoten(2 - 1, 3 - 1, 4 - 1, PotenDir::rightW) << '\n';
		std::cout << "idw(4, 3, 2) = " << Simulator::IsBlockPoten(4 - 1, 3 - 1, 2 - 1, PotenDir::backW) << '\n';
		std::cout << "iew(3, 2, 1) = " << Simulator::IsBlockPoten(3 - 1, 2 - 1, 1 - 1, PotenDir::frontW) << '\n';
		std::cout << "ifw(5, 1, 5) = " << Simulator::IsBlockPoten(5 - 1, 1 - 1, 5 - 1, PotenDir::topW) << '\n';
		std::cout << "igw(4, 2, 4) = " << Simulator::IsBlockPoten(4 - 1, 2 - 1, 4 - 1, PotenDir::bottomW) << '\n';
		std::cout << "ifw(3, 3, 3) = " << Simulator::IsBlockPoten(3 - 1, 3 - 1, 3 - 1, PotenDir::topW) << '\n';
		std::cout << "igw(2, 2, 2) = " << Simulator::IsBlockPoten(2 - 1, 2 - 1, 2 - 1, PotenDir::bottomW) << "\n\n";

		for (int k = 0; k < nz; k++)
		{
			std::cout << "k" << k + 1 << "\n";
			for (int j = 0; j < ny; j++)
			{
				std::cout << "j" << j + 1 << " ";
				for (int i = 0; i < nx; i++)
					std::cout << Simulator::GetBlockPressure(i, j, k) << " ";
				std::cout << '\n';
			}
			std::cout << "\n";
		}
	}
}