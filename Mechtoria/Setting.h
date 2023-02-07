#pragma once
#include "Vec2.h"

namespace physics 
{
	enum class component
	{
		GRAVITY,
		VOXEL
	};
	enum class directionBlock : int
	{
		NO_DIRECTION = 0,
		NORTH = 1,
		EAST = 2,
		SOUTH = 4,
		WEST = 8
	};
}

namespace state 
{
	enum class type
	{
		MAIN_MENU,
		TOP_DOWN,
		SIDE_SCROLL,
		RTS,
		GAMEPLAY,
		QUADTREE,
		SKIP
	};
}

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
	inline constexpr int screenWidth = 512;
	inline constexpr int screenHeight = 512;
	inline constexpr int FPS = 60;
	// Board settings
	inline constexpr int cellSize = 23;
	inline constexpr int padding = 2;
	inline constexpr Vec2<int> boardPosition = {250,70};
	inline constexpr Vec2<int> boardDimentions = {10,20};
	// voxel Settings
	inline constexpr voxel::DrawType voxelType = voxel::DrawType::CUBIC;
	inline constexpr bool DEBUG_QUADTREE = false;
	//Physics
	namespace physics
	{
		inline constexpr Vec2<float> gravityVec{ 0.f,2.f };
		inline constexpr Vec2<float> inputMaxVec{ 1.f,1.f };
		inline constexpr Vec2<float> inputMinVec{ -1.f,-1.f };
	}

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
		PHYSICSHANDLER,
		FINITE_STATE_MACHINE,
		FINITE_STATE_TOPDOWN,
		FINITE_STATE_GAMEPLAY,
		FINITE_STATE_MAINMENU,
		FINITE_STATE_SIDESCROLL,
		FINITE_STATE_RTS,
		FINITE_STATE_QUADTREE,
		VOXEL_CHONK,
		PLAYER_ACTOR
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