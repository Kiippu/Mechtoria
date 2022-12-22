#pragma once

#include "QuadTreeV3.h"
#include "Quad.h"

namespace worldCreator
{
	static void FlatSurfaceWorld(QuadTreeV3* quadtree)
	{
		auto dims = quadtree->GetBounds();

		Quad quad{ {dims.topLeft.GetX(), 0.f}, {dims.dimentions.GetX(), dims.dimentions.GetY()/2}};
		quadtree->InsertVoxels(quad);
	}
}

