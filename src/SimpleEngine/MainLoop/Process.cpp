#include "../include/MainLoop/Process.hpp"


/*
==================
Process::RemoveChild
==================
*/
StrongProcessPtr Process::RemoveChild() {
	if (m_childProcess) {
		auto child = m_childProcess;  // this keeps the child from getting destroyed when we clear it
		child.reset();
		return child;
	}

	return StrongProcessPtr();
}

/*
==================
Process::AttachChild
==================
*/
void Process::AttachChild(StrongProcessPtr child) {
	if (m_childProcess) {
		m_childProcess->AttachChild(child);
	} else {
		m_childProcess = child;
	}
}

/*
==================
Process::Pause
==================
*/
void Process::Pause() {
	if (m_state == State::Running) {
		m_state = State::Paused;
	}
}

/*
==================
Process::UnPause
==================
*/
void Process::UnPause() {
	if (m_state == State::Paused) {
		m_state = State::Running;
	}
}
