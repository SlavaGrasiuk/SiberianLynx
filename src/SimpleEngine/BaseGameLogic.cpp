#include "include\BaseGameLogic.hpp"



/*
==================
BaseGameLogic::BaseGameLogic
==================
*/
BaseGameLogic::BaseGameLogic() : m_state(BaseGameState::Initializing) {

}

/*
==================
BaseGameLogic::~BaseGameLogic
==================
*/
BaseGameLogic::~BaseGameLogic() {

}

/*
==================
BaseGameLogic::GetActor
==================
*/
WeakActorPtr BaseGameLogic::GetActor(const ActorId id) {
	return WeakActorPtr();
}

/*
==================
BaseGameLogic::DestroyActor
==================
*/
void BaseGameLogic::DestroyActor(const ActorId actorId) {

}

/*
==================
BaseGameLogic::OnUpdate
==================
*/
void BaseGameLogic::OnUpdate(const float deltaTimeMs) {
	switch (m_state) {
		case BaseGameState::Initializing:
			break;

		case BaseGameState::MainMenu:
			break;

		case BaseGameState::LoadingGameEnvironment:
			break;

		case BaseGameState::SpawningPlayersActors:
			break;

		case BaseGameState::Running:
			//update process manager

			//update game physics
			break;

		default:
			break;
	}

	for (auto &view : m_gameViews) {
		view->VOnUpdate(deltaTimeMs);
	}

	//update actors
}

/*
==================
BaseGameLogic::ChangeState
==================
*/
void BaseGameLogic::ChangeState(BaseGameState newState) {

}

/*
==================
BaseGameLogic::GetGameViewList
==================
*/
GameViewList & BaseGameLogic::GetGameViewList() {
	return m_gameViews;
}

/*
==================
BaseGameLogic::RenderDiagnostic
==================
*/
void BaseGameLogic::RenderDiagnostic() {

}
