#include "include\BaseGameLogic.hpp"



/*
==================
BaseGameLogic::BaseGameLogic
==================
*/
BaseGameLogic::BaseGameLogic() : m_state(BaseGameState::Initializing), m_processManager(new(std::nothrow) ProcessManager) {

}

/*
==================
BaseGameLogic::~BaseGameLogic
==================
*/
BaseGameLogic::~BaseGameLogic() {
	while (!m_gameViews.empty()) {
		m_gameViews.pop_front();
	}
	delete m_processManager;

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
			m_processManager->UpdateProcesses(deltaTimeMs);

			//update game physics
			break;

		default:
			break;
	}

	for (auto &view : m_gameViews) {
		view->OnUpdate(deltaTimeMs);
	}

	//update actors
}

/*
==================
BaseGameLogic::ChangeState
==================
*/
void BaseGameLogic::ChangeState(BaseGameState newState) {


	m_state = newState;
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
BaseGameLogic::Init
==================
*/
bool BaseGameLogic::Init() {
	return true;
}

/*
==================
BaseGameLogic::AddView
==================
*/
void BaseGameLogic::AddView(std::shared_ptr<IGameView> view, ActorId actorId) {
	const int viewId = static_cast<int>(m_gameViews.size());
	m_gameViews.push_back(view);
	view->OnAttach(viewId, actorId);
}

/*
==================
BaseGameLogic::RemoveView
==================
*/
void BaseGameLogic::RemoveView(std::shared_ptr<IGameView> view) {
	m_gameViews.remove(view);
}

/*
==================
BaseGameLogic::RenderDiagnostic
==================
*/
void BaseGameLogic::RenderDiagnostic() {

}
