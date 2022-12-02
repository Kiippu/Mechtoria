#include "VoxelChonk.h"

VoxelChonk::VoxelChonk(VoxelChonkConfiguration _config)
	: m_config(_config),
	m_voxelWorld(std::make_unique<QuadTree<Voxel>>(TreeSize::SQUARED_16384, Vec2<float>{0.f, 0.f})),
	Visual2DDynamicNode(node::type::VOXEL_CHONK, node::renderLayer::VOXEL)
{
}

void VoxelChonk::Draw()
{

}
