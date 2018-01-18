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
	//virtual StrongActorPtr CreateActor(const std::string &actorResource, TiXmlElement *overrides, const Mat4x4 *initialTransform = NULL, const ActorId serversActorId = INVALID_ACTOR_ID) = 0;
	virtual void DestroyActor(const ActorId actorId) = 0;
	//virtual bool LoadGame(const char* levelResource) = 0;
	//virtual void SetProxy() = 0;
	virtual void OnUpdate(float time, float elapsedTime) = 0;
	virtual void ChangeState(enum class BaseGameState newState) = 0;
	//virtual void MoveActor(const ActorId id, Mat4x4 const &mat) = 0;
	//virtual std::shared_ptr<IGamePhysics> GetGamePhysics(void) = 0;
};
