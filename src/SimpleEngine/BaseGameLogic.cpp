#include "include\BaseGameLogic.hpp"



/*
==================
BaseGameLogic::BaseGameLogic
==================
*/
BaseGameLogic::BaseGameLogic() {

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

}

/*
==================
BaseGameLogic::ChangeState
==================
*/
void BaseGameLogic::ChangeState(BaseGameState newState) {

}
