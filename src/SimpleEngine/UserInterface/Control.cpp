#include "Control.hpp"
#include <cstring>

/*
==================
Control::Control
==================
*/
Control::Control(Dialog * pDialog) noexcept :
m_pDialog(pDialog),
m_ID(0),
m_nHotkey(0),
m_Index(0),
m_pUserData(nullptr),
m_bEnabled(true),
m_bVisible(true),
m_bMouseOver(false),
m_bHasFocus(false),
m_bIsDefault(false),
m_x(0),
m_y(0),
m_width(0),
m_height(0)
{
	memset(&m_rcBoundingBox, 0, sizeof m_rcBoundingBox);
}

/*
==================
Control::UpdateRects
==================
*/
void Control::UpdateRects() {
	m_rcBoundingBox.left = m_x;
	m_rcBoundingBox.top = m_y;
	m_rcBoundingBox.right = m_x + m_width;
	m_rcBoundingBox.bottom = m_y + m_height;
}
