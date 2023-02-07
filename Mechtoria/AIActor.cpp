#include "AIActor.h"
#include "Setting.h"
#include "CollisionVolume.h"
#include "PhysicsHandler.h"

AIActor::AIActor(IState* state, node::renderLayer renderType, node::entity entityType)
	: GameObjectDynamic(state, renderType, entityType)
{

}

void AIActor::Initialization()
{

	CollisionVolume::CollisionVolumeConfiguration configVol
	{
		CollisionVolume::VolumeType::rectangle,
		{ 20.f,20.f },
		RED,
		BLACK,
		2,
		state::type::RTS
	};
	auto collisionVolume = std::make_shared<CollisionVolume>(configVol);
	collisionVolume->SetName("ai_actor");
	addChildNode(collisionVolume);

	//CollisionVolume* vol, Transform* trans
	auto physicsHandler = std::make_shared<PhysicsHandler>(this);
	addChildNode(physicsHandler);

}
