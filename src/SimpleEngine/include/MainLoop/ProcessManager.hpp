#pragma once

#include <list>
#include "Process.hpp"

/*
===============================================================================



===============================================================================
*/
class ProcessManager {
	using ProcessList = std::list<StrongProcessPtr>;

	ProcessList m_processList;

public:
	// construction
	~ProcessManager();

	// interface
	unsigned int UpdateProcesses(const float deltaMs);		// updates all attached processes
	WeakProcessPtr AttachProcess(StrongProcessPtr process);	// attaches a process to the process mgr
	void AbortAllProcesses(const bool immediate);

	// accessors
	auto GetProcessCount() const {
		return m_processList.size();
	}
};
