#pragma once
#include "Vec2.h"
#include <memory>
#include <vector>
#include "Node.h"
#include "FiniteStateMachine.h"
#include "Quad.h"


class QuadTreeV2 : RendableNode
{
public:
	QuadTreeV2(Quad boundry, int capacity, int voxelDepth = 5, node::renderLayer renderLayer = node::renderLayer::SKIP_NODE)
		: m_boundry(boundry), 
		m_capacity(capacity), 
		m_isDivided(false),
		m_voxelState(0),
		m_voxelDepth(voxelDepth),
		RendableNode(renderLayer, state::type::QUADTREE )
	{
		m_points.reserve(m_capacity);
	}

	virtual void Draw() override;
	// quadtree specific
	bool Insert(Vec2<float> point);
	void Query(Quad range, std::vector<Vec2<float>>& pointsFound);

	// voxel specific
	void QueryVoxels(Quad range, std::vector<QuadTreeV2>& voxelsFound);
	void InsertVoxels(Quad range, int depth = INT_MIN);
	void RemoveVoxels(Quad range, int depth = INT_MIN);
	void ActivateVoxel(state::type state);
	void DeactivateVoxel();


private:
	Quad m_boundry;
	const int m_capacity;
	// subdivisions
	std::unique_ptr<QuadTreeV2> m_northWest;
	std::unique_ptr<QuadTreeV2> m_northEast;
	std::unique_ptr<QuadTreeV2> m_southWest;
	std::unique_ptr<QuadTreeV2> m_southEast;
	// state
	bool m_isDivided;
	std::vector<Vec2<float>> m_points;
	// Voxel
	int m_voxelState;
	short m_voxelDepth;

private:
	void Subdivide(bool isVoxel = false);
	void DrawDebugQuad();
	void DrawVoxels();
	void PruneTree();
	bool isChildValid(QuadTreeV2* child);

};