#include "CollisionLogic.hpp"
#include <SimpleEngineApp.hpp>


const EventType EventDataStartGame::eventType = 0xE01120C9;


/*
==================
CollisionLogic::CollisionLogic
==================
*/
CollisionLogic::CollisionLogic() {
	RegisterAllDelegates();
}

/*
==================
CollisionLogic::~CollisionLogic
==================
*/
CollisionLogic::~CollisionLogic() {
	RemoveAllDelegates();
}

/*
==================
CollisionLogic::StartGameDelegate
==================
*/
void CollisionLogic::StartGameDelegate(IEventPtr eventData) {
	auto eventCastData = std::static_pointer_cast<EventDataStartGame>(eventData);

	switch (eventCastData->GetDifficulty()) {
		case GameDifficulty::EASY:
			ChangeState(BaseGameState::Running);
			break;

		case GameDifficulty::NORMAL:
			break;
		
		case GameDifficulty::HARD:
			break;
	}
}

/*
==================
CollisionLogic::RegisterAllDelegates
==================
*/
void CollisionLogic::RegisterAllDelegates() {
	auto eventManager = g_app->GetEventManager();
	
	eventManager->AddListener(EventListenerDelegate::create<CollisionLogic, &CollisionLogic::StartGameDelegate>(this), EventDataStartGame::eventType);
}

/*
==================
CollisionLogic::RemoveAllDelegates
==================
*/
void CollisionLogic::RemoveAllDelegates() {
	auto eventManager = g_app->GetEventManager();

	eventManager->RemoveListener(EventListenerDelegate::create<CollisionLogic, &CollisionLogic::StartGameDelegate>(this), EventDataStartGame::eventType);
}
