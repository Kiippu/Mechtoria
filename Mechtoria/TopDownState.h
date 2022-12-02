#pragma once
#include "FiniteStateMachine.h"
#include "VoxelChonk.h"

class GamePlayState;
struct Voxel;

class TopDownState : public IState
{
public:
	TopDownState(FiniteStateMachine* _parent, GamePlayState* _parentState);
	virtual ~TopDownState() {};

	virtual void Initialization() override;
	virtual void Deinitialization() override {};

private:

	GamePlayState* m_gamePlayState;
	std::unique_ptr<VoxelChonk> m_voxelWorld;

};