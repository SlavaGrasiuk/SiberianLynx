#pragma once

#include "Interfaces.hpp"

enum class BaseGameState {
	Invalid,
	Initializing,
	MainMenu,
	LoadingGameEnvironment,
	SpawningPlayersActors,
	Running
};

/*
===============================================================================

	Base class for game logic. List of game views stored here.

===============================================================================
*/
class BaseGameLogic: public IGameLogic {

public:
	BaseGameLogic();
	~BaseGameLogic();

	virtual WeakActorPtr GetActor(const ActorId id);
	virtual void DestroyActor(const ActorId actorId);
	virtual void OnUpdate(const float deltaTimeMs);
	virtual void ChangeState(BaseGameState newState);
};

