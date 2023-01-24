#pragma once
#include "Node.h"
#include "Quad.h"
#include "GameObjectDynamic.h"
#include "CollisionVolume.h"
#include "Setting.h"


class QuadTreeV3 : RendableNode
{
public:
	QuadTreeV3(Quad boundry, int capacity, state::type state, int voxelDepth = 5, node::renderLayer renderLayer = node::renderLayer::SKIP_NODE)
		: m_boundry(boundry),
		m_capacity(capacity),
		m_isDivided(false),
		m_voxelState(0),
		m_voxelDepth(voxelDepth),
		m_ownerState(state),
		RendableNode( renderLayer, state)
	{
		m_points.reserve(m_capacity);
	}

	virtual void Draw() override;
	// quadtree specific
	bool Insert(GameObjectDynamic* obj, CollisionVolume* vol = nullptr);
	void Query(Quad range, std::vector<GameObjectDynamic*>& pointsFound);
	void ClearInserts();

	// voxel specific
	void QueryVoxels(Quad range, std::vector<QuadTreeV3*>& voxelsFound, int depth = INT_MIN);
	void InsertVoxels(Quad range, int depth = INT_MIN);
	void RemoveVoxels(Quad range, int depth = INT_MIN);
	//void ActivateVoxel(state::type state);
	//void DeactivateVoxel();
	const Quad& GetBounds() const { return m_boundry; };
	bool AreChildrenActive() const;


private:
	Quad m_boundry;
	const int m_capacity;
	// subdivisions
	std::unique_ptr<QuadTreeV3> m_northWest;
	std::unique_ptr<QuadTreeV3> m_northEast;
	std::unique_ptr<QuadTreeV3> m_southWest;
	std::unique_ptr<QuadTreeV3> m_southEast;
	// state
	bool m_isDivided;
	std::vector<GameObjectDynamic*> m_points;
	// Voxel
	int m_voxelState;
	short m_voxelDepth;
	state::type m_ownerState;

private:
	void Subdivide(bool isVoxel = false);
	void DrawDebugQuad();
	void DrawVoxels();
	void PruneTree();
	bool isChildValid(QuadTreeV3* child);
	state::type getState() const { return m_ownerState; };

};

