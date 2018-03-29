#pragma once

#include "../include/EventManager/EventManager.hpp"
#include <list>
#include <map>

/*
===============================================================================

		SimpleEngine's event manager. For each registered event type it manages
	list of the delegates, called when the event of this type is recieved. 

		

===============================================================================
*/
class EventManager: public IEventManager {
	using EventListenerList = std::list<EventListenerDelegate>;
	using EventListenerMap = std::map<EventType, EventListenerList>;
	using EventQueue = std::list<IEventPtr>;

	static constexpr size_t queuesNum = 2;

	EventListenerMap m_eventListeners;
	EventQueue m_eventQueues[queuesNum];
	size_t m_activeQueue;

public:
	EventManager(): m_activeQueue(0) {}

	virtual bool AddListener(const EventListenerDelegate &eventDelegate, const EventType eventType) override;

	virtual bool RemoveListener(const EventListenerDelegate &eventDelegate, const EventType eventType) override;

	virtual bool TriggerEvent(const IEventPtr event) override;

	virtual bool QueueEvent(const IEventPtr event) override;

	virtual bool AbortEvent(const EventType type, const bool allOfType = false) override;

	virtual bool Update(const float maxMs = infinity) override;
};
