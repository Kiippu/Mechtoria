#pragma once

#include "QuadTreeV3.h"
#include "Quad.h"
#include "SimplexNoise.h"



namespace worldCreator
{
	static void FlatSurfaceWorld(QuadTreeV3* quadtree)
	{
		auto dims = quadtree->GetBounds();

		Quad quad{ {dims.topLeft.GetX()/2, 0.f}, {dims.dimentions.GetX(), dims.dimentions.GetY() / 2} };
		quadtree->InsertVoxels(quad);
	}

	static void FillWorld(QuadTreeV3* quadtree)
	{
		quadtree->InsertVoxels(quadtree->GetBounds());
	}


	static void SimpleNoise2DSurfaceWorld(QuadTreeV3* quadtree)
	{
		SimplexNoise simplex;
		auto dims = quadtree->GetBounds();

		for (int x = (int)dims.topLeft.GetX(); x < dims.bottomRight.GetX(); ++x)
		{
			float xLength = 512 + (float)x;
			float halfWorld = dims.dimentions.GetY() / 2;
			float noise = simplex.noise(xLength /dims.topLeft.GetX());

			Vec2<float> topLeft{ (float)x, noise * 100 };
			Vec2<float> bounds{ 1.f, dims.dimentions.GetY() };

			Quad quad{ topLeft, bounds};
			quadtree->InsertVoxels(quad);
		}
	}


	static void FractalNoise2DSurfaceWorld(QuadTreeV3* quadtree)
	{
		float frequency = 5.0f;
		float amplitude = 1.0f;
		float lacunarity = 3.0f;
		float persistence = 0.35f;
		SimplexNoise simplex{ frequency, amplitude, lacunarity, persistence };
		size_t octaves{ 4 };
		auto dims = quadtree->GetBounds();
		const float halfWorld = dims.dimentions.GetY() / 2;

		// TODO:	add scale to x attribute
		//			mabe make the voxel area smaller
		//			maybe make voxels begin from 0 and not -half width
		//			
		//			Make another scene to just test noise poitions!!!

		for (int x = (int)dims.topLeft.GetX(); x < dims.bottomRight.GetX(); ++x)
		{
			float xLength = halfWorld + (float)x;
			float noise = simplex.fractal( octaves, xLength /dims.topLeft.GetX());

			Vec2<float> topLeft{ (float)x, noise * 100 };
			Vec2<float> bounds{ 1.f, dims.dimentions.GetY() };

			Quad quad{ topLeft, bounds};
			quadtree->InsertVoxels(quad);
		}
	}
}

