#pragma once

enum class Coor
{
	i = 0, j, k;
};

struct Block
{
	float dx, dy, dz; // Dimensions
	float p; // Pressure
	float sw; // Water saturation
};