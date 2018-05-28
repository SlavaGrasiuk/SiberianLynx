#include "../include/UserInterface/Dialog.hpp"
#include "Control.hpp"
#include "Button.hpp"
#include "../include/SimpleEngineApp.hpp"
#include <SDL2\SDL.h>


/*
==================
MainMenuView::MainMenuView
==================
*/
Dialog::Dialog() {

}

/*
==================
MainMenuView::MainMenuView
==================
*/
void Dialog::OnRender(const float deltaTimeMs) {
	if (m_visible) {
		for (auto control : m_controls) {
			control->Render(deltaTimeMs);
		}
	}
}

/*
==================
Dialog::OnUpdate
==================
*/
void Dialog::OnUpdate(const float deltaTimeMs) {

}

/*
==================
Dialog::OnMsgProc
==================
*/
bool Dialog::OnMsgProc(const SDL_Event * const msg) {
	if (!m_visible) {
		return false;
	}

	bool processed = false;

	switch (msg->type) {
		case SDL_EventType::SDL_KEYDOWN:
		case SDL_EventType::SDL_KEYUP:
			break;

		case SDL_EventType::SDL_MOUSEMOTION:
		case SDL_EventType::SDL_MOUSEBUTTONUP:
		case SDL_EventType::SDL_MOUSEBUTTONDOWN: {
				Point mousePoint;

				if (msg->type == SDL_EventType::SDL_MOUSEMOTION) {
					mousePoint.x = msg->motion.x;
					mousePoint.y = msg->motion.y;
				} else {
					mousePoint.x = msg->button.x;
					mousePoint.y = msg->button.y;
				}

				auto control = GetControlAtPoint(mousePoint);
				if (control) {
					if (control->HandleMouse(msg)) {
						processed = true;
						break;
					}
				}

				// Still not handled, hand this off to the dialog.
				if (msg->type == SDL_EventType::SDL_MOUSEMOTION) {
					HandleMouse(mousePoint);
				}
			}
			break;

		case SDL_EventType::SDL_FINGERMOTION:
		case SDL_EventType::SDL_FINGERUP:
		case SDL_EventType::SDL_FINGERDOWN:

			break;

		default:
			break;
	}

	return processed;
}

/*
==================
Dialog::SendEvent
==================
*/
void Dialog::SendEvent(DialogEventType nEvent, Control * pControl) {
	if (m_callback) {
		m_callback(nEvent, pControl->GetID(), m_callbackUserContext);
	}
}

/*
==================
Dialog::AddButton
==================
*/
bool Dialog::AddButton(int ID, const int x, const int y, const std::string &textureName) {
	auto button = std::make_shared<Button>(this, textureName);
	if (!button) {
		return false;
	}

	button->SetID(ID);
	button->SetLocation(x, y);

	return AddControl(button);
}

/*
==================
Dialog::AddButton
==================
*/
bool Dialog::AddButton(int ID, const int y, const std::string &textureName) {
	auto button = std::make_shared<Button>(this, textureName);
	if (!button) {
		return false;
	}

	button->SetID(ID);
	const int x = g_app->GetWindowWidth() / 2 - button->m_width / 2;
	button->SetLocation(x, y);

	return AddControl(button);
}

/*
==================
Dialog::GetControlAtPoint
==================
*/
std::shared_ptr<Control> Dialog::GetControlAtPoint(const Point & pt) const {
	for (auto control : m_controls) {
		if (control->GetVisible() && control->GetEnabled() && control->ContainsPoint(pt)) {
			return control;
		}
	}

	return std::shared_ptr<Control>();
}

/*
==================
Dialog::HandleMouse
==================
*/
void Dialog::HandleMouse(const Point & pt) {
	// Figure out which control the mouse is over now
	auto pControl = GetControlAtPoint(pt);

	// If the mouse is still over the same control, nothing needs to be done
	if (pControl == m_pControlMouseOver) {
		return;
	}

	// Handle mouse leaving the old control
	if (m_pControlMouseOver) {
		m_pControlMouseOver->OnMouseLeave();
	}

	// Handle mouse entering the new control
	m_pControlMouseOver = pControl;
	if (pControl) {
		m_pControlMouseOver->OnMouseEnter();
	}
}

/*
==================
Dialog::AddControl
==================
*/
bool Dialog::AddControl(std::shared_ptr<Control> control) {
	control->m_Index = m_controls.size();
	m_controls.push_back(control);

	return true;
}
