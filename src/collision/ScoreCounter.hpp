#pragma once


#include <Interfaces.hpp>
#include <EventManager/Event.hpp>


/*
===============================================================================
	
	Receive event with score and draw it.

===============================================================================
*/
class ScoreCounter : public IScreenElement {
	std::shared_ptr<ResHandle> m_digitsTexture;
	bool m_visible;
	int m_x, m_y, m_digitWidth, m_maxDigitCount;

	static constexpr int DIGITS_COUNT = 10;		//number of frames in texture
	static constexpr int MAX_DIGITS_COUNT = 6;		//max number of digits on screen
	int frameNums[MAX_DIGITS_COUNT];

	int m_score;

public:
	ScoreCounter(const int x, const int y);
	~ScoreCounter();

	virtual void OnRender(const float deltaTimeMs) override;
	virtual void OnUpdate(const float deltaTimeMs) override {}

	virtual int GetZOrder() const {
		return 1;
	}
	virtual void SetZOrder(const int zOrder) override {}

	virtual bool IsVisible() const override {
		return m_visible;
	}
	virtual void SetVisible(bool visible) override {
		m_visible = visible;
	}

	virtual bool OnMsgProc(const SDL_Event * const msg) override final {}

public:
	void ScoreUpdateDelegate(IEventPtr eventData);
};


/*
===============================================================================

	Event data for update score

===============================================================================
*/
class EventDataUpdateScore : public BaseEvent {
	int m_score;

public:
	static const EventType eventType;

	explicit EventDataUpdateScore(int score) : m_score(score) {}

	virtual EventType GetEventType() const override {
		return eventType;
	}

	virtual IEventPtr Copy() const override {
		return IEventPtr(new(std::nothrow) EventDataUpdateScore(m_score));
	}

	virtual const char* GetName() const override {
		return "EventDataUpdateScore";
	}

	auto GetScore() const {
		return m_score;
	}
};