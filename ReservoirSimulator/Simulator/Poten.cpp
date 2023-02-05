#include "Poten.hpp"

const bool Poten::IsPoten(PotenDir potenDir) const
{
	return (poten & Bits(potenDir));
}

void Poten::SetPoten(PotenDir potenDir, bool isFlowing)
{
	if (isFlowing)
		poten |= Bits(potenDir);
	else
		poten &= ~Bits(potenDir);
}

const unsigned char Poten::Bits(PotenDir potenDir) const
{
	switch (potenDir)
	{
	case PotenDir::topW:
		return 0b1000'0000;
	case PotenDir::bottomW:
		return 0b0100'0000;
	case PotenDir::frontW:
		return 0b0010'0000;
	case PotenDir::backW:
		return 0b0001'0000;
	case PotenDir::leftW:
		return 0b0000'1000;
	case PotenDir::rightW:
		return 0b0000'0100;
	case PotenDir::topO:
		return 0b0000'0010;
	case PotenDir::bottomO:
		return 0b0000'0001;
	default:
		return 0x0;
	}
}