#include "Model.hpp"
#include "Block.hpp"

std::shared_ptr<Model> Model::Create(const std::shared_ptr<Data>& data)
{
    return std::make_shared<Model>(data);
}

Model::Model(const std::shared_ptr<Data>& data)
    : m_OOIP(0.0f), m_OGIP(0.0f)
{
    InitializeBlocks(data);
}

void Model::InitializeBlocks(const std::shared_ptr<Data>& data)
{
    // Get data content
    const auto& content = data->GetContent();

    // Initialize data
    m_Nx = content.reservoir.nx;
    m_Ny = content.reservoir.ny;
    m_Nz = content.reservoir.nz;

    m_Tx = content.reservoir.tx;
    m_Ty = content.reservoir.ty;
    m_Tz = content.reservoir.tz;

    // Prepare for input data
    float dx = m_Tx / m_Nx;
    float dy = m_Ty / m_Ny;
    float dz = m_Tz / m_Nz;

    float pinit = content.reservoir.pinit;
    float swi = content.reservoir.swi;
    float sgi = 0.0f;

    // Initialize m_Blocks
    m_Blocks = Block3D(m_Nx, Block2D(m_Ny, Block1D()));

    // Loop through blocks to fill initial data
    for (int i = 0;i < m_Nx;i++)
        for (int j = 0;j < m_Ny;j++)
            for (int k = 0; k < m_Nz; k++)
            {
                m_Blocks[i][j].push_back({ pinit, swi, sgi, dx, dy, dz });
            }
}
