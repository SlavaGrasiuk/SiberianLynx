#pragma once

#include <cstdint>

class Dialog;
union SDL_Event;
struct SDL_KeyboardEvent;

struct Point {
	long int  x, y;
};

struct Rect {
	long int left, top, right, bottom;
};

/*
===============================================================================

	Base class for UI controls.
	Control represented as abstract rectangle.
	Code from DXUTgui.h

===============================================================================
*/
class Control {
public:
	Control(Dialog* pDialog = nullptr) noexcept;
	virtual ~Control() = default;

	virtual bool OnInit() {
		return true;
	}

	virtual void Refresh() {
		m_bMouseOver = false;
		m_bHasFocus = false;
	}

	virtual void Render(float deltaTimeMs) = 0;

	virtual bool MsgProc(const SDL_Event * const msg) {
		return false;
	}

	virtual bool HandleKeyboard(const SDL_KeyboardEvent &msg) {
		return false;
	}

	virtual bool HandleMouse(const SDL_Event * const msg) {
		return false;
	}

	virtual bool CanHaveFocus() {
		return false;
	}
	virtual void OnFocusIn() {
		m_bHasFocus = true;
	}
	virtual void OnFocusOut() {
		m_bHasFocus = false;
	}
	virtual void OnMouseEnter() {
		m_bMouseOver = true;
	}
	virtual void OnMouseLeave() {
		m_bMouseOver = false;
	}
	virtual void OnHotkey() {}

	virtual bool ContainsPoint(const Point& pt) {
		return (m_rcBoundingBox.left < pt.x && pt.x < m_rcBoundingBox.right && m_rcBoundingBox.top < pt.y && pt.y < m_rcBoundingBox.bottom);
	}

	virtual void SetEnabled(bool bEnabled) {
		m_bEnabled = bEnabled;
	}
	virtual bool GetEnabled() const {
		return m_bEnabled;
	}
	virtual void SetVisible(bool bVisible) {
		m_bVisible = bVisible;
	}
	virtual bool GetVisible() const {
		return m_bVisible; 
	}

	//UINT GetType() const { return m_Type; }

	int GetID() const {
		return m_ID;
	}

	void SetID(int ID) {
		m_ID = ID;
	}

	void SetLocation(int x, int y) {
		m_x = x;
		m_y = y;
		UpdateRects();
	}

	virtual void SetSize(int width, int height) {
		m_width = width;
		m_height = height;
		UpdateRects();
	}

	void SetHotkey(int32_t nHotkey) {
		m_nHotkey = nHotkey;
	}

	int32_t GetHotkey() const {
		return m_nHotkey;
	}

	void SetUserData(void* pUserData = nullptr) {
		m_pUserData = pUserData;
	}

	void* GetUserData() const {
		return m_pUserData;
	}

	//virtual void SetTextColor(_In_ DWORD Color);
	//CDXUTElement* GetElement(_In_ UINT iElement) const { return m_Elements[iElement]; }
	//HRESULT SetElement(_In_ UINT iElement, _In_ CDXUTElement* pElement);

	bool m_bVisible;                // Shown/hidden flag
	bool m_bMouseOver;              // Mouse pointer is above control
	bool m_bHasFocus;               // Control has input focus
	bool m_bIsDefault;              // Is the default control

									
	int m_x, m_y;					// Size, scale, and positioning members
	int m_width, m_height;

	// These members are set by the container
	Dialog* m_pDialog;    // Parent container
	unsigned int m_Index; // Index within the control list

	//std::vector<CDXUTElement*> m_Elements;  // All display elements

protected:
	virtual void UpdateRects();

	int m_ID;					// ID number
	//DXUT_CONTROL_TYPE m_Type;  // Control type, set once in constructor  
	int32_t m_nHotkey;         // Virtual key code for this control's hotkey
	void* m_pUserData;         // Data associated with this control that is set by user.

	bool m_bEnabled;           // Enabled/disabled flag

	Rect m_rcBoundingBox;      // Rectangle defining the active region of the control
};
