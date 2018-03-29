#include "../include/MainLoop/ProcessManager.hpp"


/*
==================
Process::RemoveChild
==================
*/
ProcessManager::~ProcessManager() {
	m_processList.clear();
}

/*
==================
ProcessManager::UpdateProcesses

	The process update tick.  Called every logic tick.  This function returns the number of process chains that
	succeeded in the upper 16 bits and the number of process chains that failed or were aborted in the lower 16 bits.
==================
*/
unsigned int ProcessManager::UpdateProcesses(const float deltaMs) {
	unsigned short int successCount = 0, failCount = 0;

	auto it = m_processList.begin();

	while (it != m_processList.end()) {
		auto currProcess = *it;	// grab the next process

		auto thisIt = it;		// save the iterator and increment the old one in case we need to remove this process from the list
		++it;

		if (currProcess->GetState() == Process::State::Uninitialized) {
			currProcess->OnInit();	// process is uninitialized, so initialize it
		}

		if (currProcess->GetState() == Process::State::Running) {	// give the process an update tick if it's running
			currProcess->OnUpdate(deltaMs);
		}
		
		if (currProcess->IsDead()) {			// check to see if the process is dead
			switch (currProcess->GetState()) {	// run the appropriate exit function
				case Process::State::Succeeded: {
						currProcess->OnSuccess();
						auto child = currProcess->RemoveChild();
						if (child) {
							AttachProcess(child);
						} else {
							successCount++;		// only counts if the whole chain completed
						}
					}
					break;

				case Process::State::Failed: {
						currProcess->OnFail();
						failCount++;
					}
					break;
					
				case Process::State::Aborted: {
						currProcess->OnAbort();
						failCount++;
					}
					break;

				default:
					break;
			}

			// remove the process and destroy it
			m_processList.erase(thisIt);
		}
	}

	return ((successCount << 16) | failCount);
}

/*
==================
ProcessManager::AttachProcess
==================
*/
WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr process) {
	m_processList.push_front(process);
	return WeakProcessPtr(process);
}

/*
==================
ProcessManager::AbortAllProcesses
==================
*/
void ProcessManager::AbortAllProcesses(const bool immediate) {
	auto it = m_processList.begin();

	while (it != m_processList.end()) {
		auto tempIt = it;
		++it;
		auto process = *tempIt;

		if (process->IsAlive()) {
			process->SetState(Process::State::Aborted);
			if (immediate) {
				process->OnAbort();
				m_processList.erase(tempIt);
			}
		}
	}
}
