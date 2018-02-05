#include "EventManager_local.hpp"
#include <chrono>


/*
==================
EventManager::AddListener
==================
*/
bool EventManager::AddListener(const EventListenerDelegate &eventDelegate, const EventType eventType) {
	auto &listenersList = m_eventListeners[eventType];

	for (auto &delegate : listenersList) {
		if (eventDelegate == delegate) {
			return false;	//Attempting to double-register a delegate
		}
	}

	listenersList.push_front(eventDelegate);
	return true;
}

/*
==================
EventManager::RemoveListener
==================
*/
bool EventManager::RemoveListener(const EventListenerDelegate &eventDelegate, const EventType eventType) {
	bool success = false;

	const auto findIt = m_eventListeners.find(eventType);
	if (findIt != m_eventListeners.end()) {
		auto &listenersList = findIt->second;

		for (auto it = listenersList.begin(); it != listenersList.end(); it++) {
			if (eventDelegate == (*it)) {
				listenersList.erase(it);
				success = true;
				break;
			}
		}
	}

	return success;
}

/*
==================
EventManager::TriggerEvent
==================
*/
bool EventManager::TriggerEvent(const IEventPtr event) {
	if (!event) {
		return false;	//early exit - invalid event
	}
	
	bool processed = false;

	const auto findIt = m_eventListeners.find(event->GetType());
	if (findIt != m_eventListeners.end()) {
		for (const auto &delegate : findIt->second) {
			delegate(event);
			processed = true;
		}
	}

	return processed;
}

/*
==================
EventManager::QueueEvent
==================
*/
bool EventManager::QueueEvent(const IEventPtr event) {
	if (!event) {
		return false;
	}

	const auto findIt = m_eventListeners.find(event->GetType());
	if (findIt != m_eventListeners.end()) {
		m_eventQueues[m_activeQueue].push_front(event);
		return true;
	}

	return false;	//there is no registered listeners for this event
}

/*
==================
EventManager::AbortEvent
==================
*/
bool EventManager::AbortEvent(const EventType type, const bool allOfType) {
	bool success = false;

	const auto findIt = m_eventListeners.find(type);
	if (findIt != m_eventListeners.end()) {
		auto &eventQueue = m_eventQueues[m_activeQueue];
		auto it = eventQueue.begin();

		while (it != eventQueue.end()) {
			auto thisIt = it;		
			it++;				//Removing an item from the queue will invalidate the iterator, so have it point to the next member

			if ((*thisIt)->GetType() == type) {
				eventQueue.erase(thisIt);
				success = true;
				if (!allOfType) {
					break;
				}
			}
		}
	}

	return success;
}

/*
==================
EventManager::Update

	Return true, if all events was processed within the specified time limit.
==================
*/
bool EventManager::Update(const float maxMs) {
	using namespace std::chrono;
	using durationMs = duration<float, std::milli>;

	time_point<high_resolution_clock, durationMs> endTimePoint;
	if (maxMs != IEventManager::infinity) {				//do not perform syscall if the time limit is not set
		endTimePoint = high_resolution_clock::now() + durationMs(maxMs);
	}

	const int queueToProcess = m_activeQueue;
	m_activeQueue = (m_activeQueue + 1) % queuesNum;
	m_eventQueues[m_activeQueue].clear();
	auto &eventQueue = m_eventQueues[queueToProcess];

	while (!eventQueue.empty()) {
		auto eventPtr = eventQueue.front();
		eventQueue.pop_front();

		const auto findIt = m_eventListeners.find(eventPtr->GetType());
		if (findIt != m_eventListeners.end()) {
			const auto &listeners = findIt->second;

			for (const auto &delegate : listeners) {
				delegate(eventPtr);
			}
		}

		if (maxMs != IEventManager::infinity) {
			const auto currentTimePoint = high_resolution_clock::now();
			if (currentTimePoint >= endTimePoint) {
				break;		//Time limit for this frame is reached.
			}
		}
	}

	const bool queueFlushed = eventQueue.empty();
	if (!queueFlushed) {
		while (!eventQueue.empty()) {
			auto eventPtr = eventQueue.back();
			eventQueue.pop_back();
			m_eventQueues[m_activeQueue].push_front(eventPtr);
		}
	}

	return queueFlushed;
}
