#pragma once

#include <memory>
#include <string>
#include <climits>

class Actor;

using WeakActorPtr = std::weak_ptr<Actor>;
using StrongActorPtr = std::shared_ptr<Actor>;
using ActorId = unsigned int;

constexpr ActorId INVALID_ACTOR_ID = 0;

union SDL_Event;

class IScreenElement {
public:
	virtual void OnRender(const float deltaTimeMs) = 0;
	virtual void OnUpdate(const float deltaTimeMs) = 0;

	virtual int GetZOrder() const = 0;
	virtual void SetZOrder(const int zOrder) = 0;
	virtual bool IsVisible() const = 0;
	virtual void SetVisible(bool visible) = 0;

	virtual bool OnMsgProc(const SDL_Event * const msg) = 0;

	virtual ~IScreenElement() = default;
	virtual bool operator<(IScreenElement const &other) const {
		return GetZOrder() < other.GetZOrder();
	}
};

constexpr int INFINITY_FAR = INT_MIN;
constexpr int INFINITY_NEAR = INT_MAX;

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
enum class GameViewType {
	HUMAN,
	REMOTE,
	AI,
	RECORDER,
	OTHER
};

class IGameView {
public:
	virtual void OnRender(const float deltaTimeMs) = 0;
	virtual GameViewType GetType() = 0;
	virtual GameViewId GetId() const = 0;
	virtual void OnAttach(GameViewId vid, ActorId aid) = 0;
	virtual bool OnMsgProc(const SDL_Event * const msg) = 0;
	virtual void OnUpdate(const float deltaTimeMs) = 0;
	virtual ~IGameView() = default;
};


struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class IPointerHandler {
public:
	virtual bool OnPointerMove(const SDL_MouseMotionEvent &event, const int radius) = 0;
	virtual bool OnPointerButton(const SDL_MouseButtonEvent &event, const int radius) = 0;
};

struct SDL_TouchFingerEvent;

class ITouchHandler {
public:
	virtual bool OnTouchEvent(const SDL_TouchFingerEvent &event, const int radius) = 0;
};


class ResHandle;

//interface to file-specific loaders
class IResourceLoader {
public:
	virtual const char *GetPattern() const = 0;
	virtual bool UseRawFile() const = 0;
	virtual bool DiscardRawBufferAfterLoad() const = 0;
	virtual bool AddNullZero() const {
		return false;
	}
	virtual size_t GetLoadedResourceSize(uint8_t *rawBuffer, size_t rawSize) = 0;
	virtual bool LoadResource(uint8_t *rawBuffer, size_t rawSize, std::shared_ptr<ResHandle> handle) = 0;
};

class IResourceFile {
public:
	virtual bool Open() = 0;
	virtual int GetRawResourceSize(const std::string &r) = 0;
	virtual size_t GetRawResource(const std::string &r, uint8_t *buffer) = 0;
	virtual int GetNumResources() const = 0;
	virtual std::string GetResourceName(int num) const = 0;
	virtual ~IResourceFile() {}
};

