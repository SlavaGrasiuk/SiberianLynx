#pragma once

#include <memory>

class Actor;

using WeakActorPtr = std::weak_ptr<Actor>;
using StrongActorPtr = std::shared_ptr<Actor>;
using ActorId = unsigned int;

const ActorId INVALID_ACTOR_ID = 0;

class IGamePhysics;

class IGameLogic {
public:
	virtual WeakActorPtr GetActor(const ActorId id) = 0;
	virtual void DestroyActor(const ActorId actorId) = 0;
	virtual void OnUpdate(const float deltaTimeMs) = 0;
	virtual void ChangeState(enum class BaseGameState newState) = 0;
};

using GameViewId = unsigned int;
union SDL_Event;

class IGameView {
public:
	virtual void VOnRender(const float deltaTimeMs) = 0;
	virtual enum class GameViewType VGetType() = 0;
	virtual GameViewId VGetId() const = 0;
	virtual void OnAttach(GameViewId vid, ActorId aid) = 0;
	virtual bool OnMsgProc(const SDL_Event * const msg) = 0;
	virtual void VOnUpdate(const float deltaTimeMs) = 0;
	virtual ~IGameView() {};
};