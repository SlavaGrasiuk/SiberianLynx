#pragma once

#include "Interfaces.hpp"
#include "../include/MainLoop/ProcessManager.hpp"
#include <list>

enum class BaseGameState {
	Invalid,
	Initializing,
	MainMenu,
	LoadingGameEnvironment,
	SpawningPlayersActors,
	Running
};

using GameViewList = std::list<std::shared_ptr<IGameView>>;

/*
===============================================================================

	Base class for game logic. List of game views stored here.

===============================================================================
*/
class BaseGameLogic: public IGameLogic {
	GameViewList m_gameViews;
	BaseGameState m_state;
	ProcessManager *m_processManager;

public:
	BaseGameLogic();
	~BaseGameLogic();

	virtual WeakActorPtr GetActor(const ActorId id) override;
	virtual void DestroyActor(const ActorId actorId) override;
	virtual void OnUpdate(const float deltaTimeMs) override;
	virtual void ChangeState(BaseGameState newState) override;

	GameViewList &GetGameViewList();

	void AttachProcess(StrongProcessPtr process) {
		if (m_processManager) {
			m_processManager->AttachProcess(process);
		}
	}

	virtual void RenderDiagnostic();
};
