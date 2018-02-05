#pragma once

#include <limits>
#include <Delegate.h>
#include "Event.hpp"

using EventListenerDelegate = SA::delegate<void(IEventPtr)>;

class IEventManager {
public:
	static constexpr float infinity = std::numeric_limits<float>::infinity();

	virtual bool AddListener(const EventListenerDelegate &eventDelegate, const EventType eventType) = 0;

	virtual bool RemoveListener(const EventListenerDelegate &eventDelegate, const EventType eventType) = 0;

	virtual bool TriggerEvent(const IEventPtr event) = 0;

	virtual bool QueueEvent(const IEventPtr event) = 0;

	virtual bool AbortEvent(const EventType type, bool allOfType = false) = 0;

	virtual bool Update(const float maxMs = infinity) = 0;
};
