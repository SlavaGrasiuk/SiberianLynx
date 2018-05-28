#pragma once

#include <cstdint>
#include <memory>

class IEvent;

using EventType = uint32_t;
using IEventPtr = std::shared_ptr<IEvent>;


class IEvent {
public:
	virtual ~IEvent() = default;
	virtual EventType GetEventType() const = 0;
	virtual float GetTimeStamp() const = 0;
	virtual IEventPtr Copy() const = 0;
	virtual const char* GetName() const = 0;
};


class BaseEvent: public IEvent {
	const float m_timeStamp;

public:
	explicit BaseEvent(const float timeStamp = 0.0f): m_timeStamp(timeStamp) {}

	float GetTimeStamp() const {
		return m_timeStamp;
	}
};
