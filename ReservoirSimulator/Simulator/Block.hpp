#pragma once

#include "Simulator.hpp"

struct Block
{
	float p, sw, sg; // Pressure (psi), water saturation (.frac), and gas saturation (.frac)
	float dx, dy, dz;

	constexpr float So() const { return 1 - sw - sg; }
	constexpr float Volume() const { return dx * dy * dz; }

	float Phi() const { return Simulator::Calc(Props::Phi, p); }
	float Bo()  const { return Simulator::Calc(Props::Bo,  p); }
	float Bg()  const { return Simulator::Calc(Props::Bg,  p); }
	float Rs()  const { return Simulator::Calc(Props::Rs,  p); }
};