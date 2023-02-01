#pragma once

#include "Data.hpp"

struct Block;
class Model
{
public:
	using Block1D = std::vector<Block>;
	using Block2D = std::vector<Block1D>;
	using Block3D = std::vector<Block2D>;

public:
	struct Coordinate { int nx, ny, nz; };
	struct Dimension { float tx, ty, tz; };

public:
	Model(const std::shared_ptr<Data>& data);

	void InitializeBlocks(const std::shared_ptr<Data>& data);

	const Coordinate GetCoordinate() const { return { m_Nx, m_Ny, m_Nz }; }
	const Dimension  GetDimension()  const { return { m_Tx, m_Ty, m_Tz }; }

	const Block3D& GetBlocks() const { return m_Blocks; }
	Block3D& SetBlocks() { return m_Blocks; }

	const float& GetOOIP() const { return m_OOIP; }
	void SetOOIP(float ooip) { m_OOIP = ooip; }

	const float& GetOGIP() const { return m_OGIP; }
	void SetOGIP(float ogip) { m_OGIP = ogip; }

	static std::shared_ptr<Model> Create(const std::shared_ptr<Data>& data);

private:
	Block3D m_Blocks;

	float m_OOIP; // [bbl]
	float m_OGIP; // [scf]

	// no of grids in x- y- and z-direction
	int m_Nx = 0;
	int m_Ny = 0;
	int m_Nz = 0;

	// dimension in x- y- and z-direction (ft)
	float m_Tx = 0.0f;
	float m_Ty = 0.0f;
	float m_Tz = 0.0f;
};