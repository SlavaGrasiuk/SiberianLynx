#pragma once

#include <memory>


class Process;
using StrongProcessPtr = std::shared_ptr<Process>;
using WeakProcessPtr = std::weak_ptr<Process>;

/*
===============================================================================

	Process class
 
	Processes are ended by one of three methods: Success, Failure, or Aborted.
		- Success means the process completed successfully.  If the process has a child, it will be attached to 
		  the process mgr.
		- Failure means the process started but failed in some way.  If the process has a child, it will be 
		  aborted.
		- Aborted processes are processes that are canceled while not submitted to the process mgr.  Depending 
		  on the circumstances, they may or may not have gotten an OnInit() call.  For example, a process can 
		  spawn another process and call AttachToParent() on itself.  If the new process fails, the child will
		  get an Abort() call on it, even though its status is RUNNING.

===============================================================================
*/
class Process {
	friend class ProcessManager;

public:
	enum class State {
		// Processes that are neither dead nor alive
		Uninitialized = 0,  // created but not running
		Removed,			// removed from the process list but not destroyed; this can happen when a process that is already running is parented to another process

		// Living processes
		Running,			// initialized and running
		Paused,				// initialized but paused

		// Dead processes
		Succeeded,			// completed successfully
		Failed,				// failed to complete
		Aborted,			// aborted; may not have started
	};

private:
	State m_state;
	StrongProcessPtr m_childProcess;

public:
	Process(): m_state(State::Uninitialized) {}
	virtual ~Process() {
		if (m_childProcess) {
			m_childProcess->OnAbort();
		}
	}

protected:
	// interface; these functions should be overridden by the subclass as needed
	virtual void OnInit() {		// called during the first update; responsible for setting the initial state (typically RUNNING)
		m_state = State::Running;
	}  
	virtual void OnUpdate(const float deltaMs) = 0;  // called every frame
	virtual void OnSuccess() = 0;	// called if the process succeeds (see below)
	virtual void OnFail() = 0;		// called if the process fails (see below)
	virtual void OnAbort() = 0;		// called if the process is aborted (see below)

public:
	// Functions for ending the process.
	void Succeed() {
		m_state = State::Succeeded;
	}
	void Fail() {
		m_state = State::Failed;
	}

	// pause
	void Pause();
	void UnPause();

	// accessors
	State GetState() const {
		return m_state;
	}
	bool IsAlive() const {
		return (m_state == State::Running || m_state == State::Paused);
	}
	bool IsDead() const {
		return (m_state == State::Succeeded || m_state == State::Failed || m_state == State::Aborted);
	}
	bool IsRemoved() const {
		return (m_state == State::Removed);
	}
	bool IsPaused() const {
		return (m_state == State::Paused);
	}

	// child functions
	void AttachChild(StrongProcessPtr child);
	StrongProcessPtr RemoveChild();		// releases ownership of the child
	StrongProcessPtr PeekChild() {		// doesn't release ownership of the child
		return m_childProcess;
	}  

private:
	void SetState(State newState) {
		m_state = newState;
	}
};
