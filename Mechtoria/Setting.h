#pragma once
#include "Vec2.h"


namespace voxel
{
	enum class DrawType
	{
		CUBIC,
		MARCHING_SQUARES_DEFAULT
	};
	enum class Mask : int
	{
		NO_VOXEL	= 0,
		NORTH_WEST	= 1,
		NORTH_EAST	= 2,
		SOUTH_WEST	= 4,
		SOUTH_EAST	= 8
	};
}

namespace settings
{
	// Window settings
	inline constexpr int screenWidth = 800;
	inline constexpr int screenHeight = 800;
	inline constexpr int FPS = 60;
	// Board settings
	inline constexpr int cellSize = 23;
	inline constexpr int padding = 2;
	inline constexpr Vec2<int> boardPosition = {250,70};
	inline constexpr Vec2<int> boardDimentions = {10,20};
	// voxel Settings
	inline constexpr voxel::DrawType voxelType = voxel::DrawType::CUBIC;
	inline constexpr bool DEBUG_QUADTREE = true;

}

namespace node
{
	enum class renderLayer
	{
		SKIP_NODE,
		BACKGROUND,
		FOREGROUND,
		GAME_ACTOR_PLAYER,
		GAME_ACTOR_STATIC,
		DEBUG_NON_GUI,
		VOXEL
	};

	enum class type {
		UNDEFINED,
		VISUAL_2D_SHAPE,
		COLLISION_2D_SHAPE,
		GAME_OBJECT,
		ENGINE_POOL,
		CONTROLLER,
		INPUTHANDLER,
		FINITE_STATE_MACHINE,
		FINITE_STATE_TOPDOWN,
		FINITE_STATE_GAMEPLAY,
		FINITE_STATE_MAINMENU,
		FINITE_STATE_SIDESCROLL,
		FINITE_STATE_QUADTREE,
		VOXEL_CHONK
	};

	enum class entity {
		UNDEFINED,
		PAWN_STATIC,
		PAWN_DYNAMIC
	};
}

namespace network
{
	enum class controllerType
	{
		NETWORK_CLIENT,
		NETWORK_SERVER,
		LOCAL
	};
}