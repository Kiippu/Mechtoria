#include "PlayerActor.h"
#include "Setting.h"
#include "CollisionVolume.h"
#include "PhysicsHandler.h"

class IState;

PlayerActor::PlayerActor(IState* state, node::renderLayer renderType, node::entity entityType)
	: GameObjectDynamic(state, renderType,entityType )
{

}

void PlayerActor::Initialization()
{

	CollisionVolume::CollisionVolumeConfiguration configVol
	{
		CollisionVolume::VolumeType::rectangle,
		{ 100.f,100.f },
		RED,
		BLACK,
		5,
		state::type::SIDE_SCROLL
	};
	auto collisionVolume = std::make_shared<CollisionVolume>(configVol);
	collisionVolume->SetName("PlayerCollision");
	addChildNode(collisionVolume);

	//CollisionVolume* vol, Transform* trans
	auto physicsHandler = std::make_shared<PhysicsHandler>(this);
	addChildNode(physicsHandler);

}
