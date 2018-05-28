#pragma once

#include <list>
#include "../Interfaces.hpp"

class Control;
struct Point;


enum class DialogEventType {
	BUTTON_CLICKED = 0x0101,
};


using CallbackType_t = void(*)(DialogEventType nEvent, int nControlID, void *m_callbackUserContext);


/*
===============================================================================

	Represent container for controls

===============================================================================
*/
class Dialog: public IScreenElement {
	std::list<std::shared_ptr<Control>> m_controls;
	std::shared_ptr<Control> m_pControlMouseOver;

	bool m_visible;
	int m_zOrder;

	CallbackType_t m_callback;
	void *m_callbackUserContext;

public:
	Dialog();
	~Dialog() = default;

	virtual void OnRender(const float deltaTimeMs) override;
	virtual void OnUpdate(const float deltaTimeMs) override;

	virtual int GetZOrder() const override {
		return m_zOrder;
	}
	virtual void SetZOrder(const int zOrder) override {
		m_zOrder = zOrder;
	}

	virtual bool IsVisible() const override {
		return m_visible;
	}
	virtual void SetVisible(bool visible) override {
		m_visible = visible;
	}

	virtual bool OnMsgProc(const SDL_Event * const msg) override;

	void SendEvent(DialogEventType nEvent, Control* pControl);

	void SetCallback(CallbackType_t callback, void* pUserContext = nullptr) {
		m_callback = callback;
		m_callbackUserContext = pUserContext;
	}

public:
	bool AddButton(int ID, const int x, const int y, const std::string &textureName);
	bool AddButton(int ID, const int y, const std::string &textureName);		//create a screen center aligned button

private:
	std::shared_ptr<Control> GetControlAtPoint(const Point &pt) const;
	void HandleMouse(const Point &pt);

	bool AddControl(std::shared_ptr<Control> control);
};

