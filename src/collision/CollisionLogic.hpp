#pragma once


#include <BaseGameLogic.hpp>
#include <EventManager\Event.hpp>


enum class GameDifficulty {
	EASY,
	NORMAL,
	HARD
};


class EventDataStartGame : public BaseEvent {
	GameDifficulty m_difficulty;

public:
	static const EventType eventType;

	explicit EventDataStartGame(GameDifficulty difficulty) : m_difficulty(difficulty) {}

	virtual EventType GetEventType() const override {
		return eventType;
	}

	virtual IEventPtr Copy() const override {
		return IEventPtr(new(std::nothrow) EventDataStartGame(m_difficulty));
	}

	virtual const char* GetName() const override {
		return "EventDataStartGame";
	}

	auto GetDifficulty() const {
		return m_difficulty;
	}
};


class CollisionLogic : public BaseGameLogic {

public:
	CollisionLogic();
	virtual ~CollisionLogic();

public:		//event delegates
	void StartGameDelegate(IEventPtr eventData);

private:
	void RegisterAllDelegates();
	void RemoveAllDelegates();
};

