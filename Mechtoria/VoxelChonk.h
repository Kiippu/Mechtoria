#pragma once
#include "Voxel.h"
#include "QuadTree.h"

class VoxelChonk : public Visual2DDynamicNode
{
public:
	struct VoxelChonkConfiguration
	{
		float voxelSize = 10.f;
	};
	VoxelChonk(VoxelChonkConfiguration _config);
	virtual void Initialization() override {};
	virtual void Deinitialization() override {};
	virtual void Draw() override;

	void DrawVoxelOutlines() {};

	void ApplyFunction() {};

private:
	std::unique_ptr<QuadTree<Voxel>> m_voxelWorld;
	VoxelChonkConfiguration m_config;
};

