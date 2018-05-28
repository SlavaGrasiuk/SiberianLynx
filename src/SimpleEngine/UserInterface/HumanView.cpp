#include "..\include\UserInterface\HumanView.hpp"
#include <SDL2\SDL.h>



/*
==================
HumanView::HumanView
==================
*/
HumanView::HumanView(): m_viewId(INVALID_VIEW_ID), m_pointerRadius(1), m_elementAdded(false) {

}

/*
==================
HumanView::~HumanView
==================
*/
HumanView::~HumanView() {
	while (!m_screenElements.empty()) {
		m_screenElements.pop_front();
	}
}

/*
==================
HumanView::OnRender
==================
*/
void HumanView::OnRender(const float deltaTimeMs) {
	if (m_elementAdded) {
		m_screenElements.sort([](const std::shared_ptr<IScreenElement> &lhs, const std::shared_ptr<IScreenElement> &rhs) {return *lhs < *rhs; });
		m_elementAdded = false;
	}
	
	for (auto el : m_screenElements) {
		if (el->IsVisible()) {
			el->OnRender(deltaTimeMs);
		}
	}
}

/*
==================
HumanView::GetType
==================
*/
GameViewType HumanView::GetType() {
	return GameViewType::HUMAN;
}

/*
==================
HumanView::GetId
==================
*/
GameViewId HumanView::GetId() const {
	return m_viewId;
}

/*
==================
HumanView::OnAttach
==================
*/
void HumanView::OnAttach(GameViewId vid, ActorId aid) {
	m_actorId = aid;
	m_viewId = vid;
}

/*
==================
HumanView::OnMsgProc
==================
*/
bool HumanView::OnMsgProc(const SDL_Event * const msg) {
	for (auto it = m_screenElements.rbegin(); it != m_screenElements.rend(); it++) {
		if ((*it)->IsVisible()) {
			if ((*it)->OnMsgProc(msg)) {
				return true;
			}
		}
	}

	bool result = false;
	switch (msg->type) {
		case SDL_EventType::SDL_MOUSEMOTION:
			if (m_mouseHandler) {
				result = m_mouseHandler->OnPointerMove(msg->motion, m_pointerRadius);
			}
			break;

		case SDL_EventType::SDL_MOUSEBUTTONUP:
		case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			if (m_mouseHandler) {
				result = m_mouseHandler->OnPointerButton(msg->button, m_pointerRadius);
			}
			break;
			
		case SDL_EventType::SDL_FINGERMOTION:
		case SDL_EventType::SDL_FINGERUP:
		case SDL_EventType::SDL_FINGERDOWN:
			if (m_touchHandler) {
				result = m_touchHandler->OnTouchEvent(msg->tfinger, m_pointerRadius);
			}
			break;

		default:
			break;
	}

	return result;
}

/*
==================
HumanView::OnUpdate
==================
*/
void HumanView::OnUpdate(const float deltaTimeMs) {
	m_processManager.UpdateProcesses(deltaTimeMs);

	for (auto el : m_screenElements) {
		el->OnUpdate(deltaTimeMs);
	}
}

/*
==================
HumanView::PushElement
==================
*/
void HumanView::PushElement(std::shared_ptr<IScreenElement> pElement) {
	m_screenElements.push_back(pElement);
	m_elementAdded = true;
}

/*
==================
HumanView::RemoveElement
==================
*/
void HumanView::RemoveElement(std::shared_ptr<IScreenElement> pElement) {
	m_screenElements.remove(pElement);
}
