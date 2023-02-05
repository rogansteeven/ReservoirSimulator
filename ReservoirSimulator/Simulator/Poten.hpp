#pragma once

#include "Enum.hpp"

struct Poten
{
	// Potential flow from current block to neighbor block
	// As there was eight posibilities, we can try by using 8 bits (char)
	// bit from left to right is indetified as follow
	// topW, bottomW, frontW, backW, leftW, rightW, topO, bottomO
	// e.g. 01000000 mean potential water flow to bottom
	// e.g. 10000010 mean potential water flow to top and oil flow to top

	unsigned char poten = 0x0;

	const bool IsPoten(PotenDir potenDir) const;
	void SetPoten(PotenDir potenDir, bool isPoten);

private:
	const unsigned char Bits(PotenDir potenDir) const;
};