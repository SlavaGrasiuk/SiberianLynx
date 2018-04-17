#pragma once

#include <memory>

class Actor;

using WeakActorPtr = std::weak_ptr<Actor>;
using StrongActorPtr = std::shared_ptr<Actor>;
using ActorId = unsigned int;

const ActorId INVALID_ACTOR_ID = 0;

class IGamePhysics;
enum class BaseGameState;

class IGameLogic {
public:
	virtual WeakActorPtr GetActor(const ActorId id) = 0;
	virtual void DestroyActor(const ActorId actorId) = 0;
	virtual void OnUpdate(const float deltaTimeMs) = 0;
	virtual void ChangeState(BaseGameState newState) = 0;
};


using GameViewId = unsigned int;
union SDL_Event;
enum class GameViewType;

class IGameView {
public:
	virtual void VOnRender(const float deltaTimeMs) = 0;
	virtual GameViewType VGetType() = 0;
	virtual GameViewId VGetId() const = 0;
	virtual void OnAttach(GameViewId vid, ActorId aid) = 0;
	virtual bool OnMsgProc(const SDL_Event * const msg) = 0;
	virtual void VOnUpdate(const float deltaTimeMs) = 0;
	virtual ~IGameView() = default;
};



class Resource;
class IResourceFile;
class ResHandle;

//interface to file-specific loaders
class IResourceLoader {
public:
	virtual std::string GetPattern() = 0;
	virtual bool UseRawFile() = 0;
	virtual bool DiscardRawBufferAfterLoad() = 0;
	virtual bool AddNullZero() {
		return false;
	}
	virtual unsigned int GetLoadedResourceSize(char *rawBuffer, unsigned int rawSize) = 0;
	virtual bool LoadResource(char *rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> handle) = 0;
};

class IResourceFile {
public:
	virtual bool Open() = 0;
	virtual int GetRawResourceSize(const std::string &r) = 0;
	virtual std::unique_ptr<uint8_t[]> GetRawResource(const std::string &r) = 0;
	virtual int GetNumResources() const = 0;
	virtual std::string GetResourceName(int num) const = 0;
	virtual ~IResourceFile() {}
};
