#pragma once
#include "raylib.h"
#include "Node.h"
#include "CollisionVolume.h"

struct VoxelDetails
{
	VoxelDetails(Color _colour = WHITE, float _resistance = 1.f)
		: colour(_colour), resistance(_resistance)
	{}
	Color colour;
	float resistance;
};

struct Voxel
{
	Voxel() 
		: m_collision(CollisionVolume::CollisionVolumeConfiguration{ CollisionVolume::VolumeType::rectangle, Vec2<float>{ 1.f,1.f } })
	{}
	float life = 1.f;
	VoxelDetails* voxelDetails = nullptr;
	CollisionVolume m_collision;
	virtual void SetCollisionDims(Vec2<float> _dims)
	{
		m_collision.SetCollisionDims(_dims);
	};
};

struct Dirt : public VoxelDetails
{
	Dirt() : VoxelDetails(BROWN, 1.f) {};
};
extern Dirt s_dirtVoxel;

struct WoodWall : public VoxelDetails
{
	WoodWall() : VoxelDetails(DARKBROWN, 2.f) {};
};
extern WoodWall s_woodWallVoxel;