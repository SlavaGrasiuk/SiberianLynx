#include "Button.hpp"
#include "../include/SimpleEngineApp.hpp"
#include "../include/Resource/ResourceManager.hpp"
#include "../include/Graphics2D/Texture.hpp"
#include "../include/Graphics2D/Renderer.hpp"
#include "../include/UserInterface/Dialog.hpp"
#include <SDL2/SDL.h>


/*
==================
Button::Button
==================
*/
Button::Button(Dialog* pDialog, const std::string &textureName) : Control(pDialog), m_texture(g_app->GetResourceMan()->GetHandle(textureName)), m_pressed(false) {
	if (m_texture) {
		auto textureExtraData = std::static_pointer_cast<TextureExtraData>(m_texture->GetExtra());

		m_height = textureExtraData->GetHeigth();
		m_width = textureExtraData->GetWidth() / STATES_COUNT;
		UpdateRects();
	}
}

/*
==================
Button::Render
==================
*/
void Button::Render(float deltaTimeMs) {
	if (m_bVisible == false || !m_texture) {
		return;
	}
	
	int nOffsetX = 0;
	int nOffsetY = 0;

	int frame = 0;		//normal state by default

	if (m_pressed) {
		frame = 2;

		nOffsetX = 1;
		nOffsetY = 2;
	} else if (m_bMouseOver) {
		frame = 1;

		nOffsetX = -1;
		nOffsetY = -2;
	}

	auto textureExtraData = std::static_pointer_cast<TextureExtraData>(m_texture->GetExtra());

	g_app->GetRenderer()->EnableAlphaBlending(textureExtraData);
	g_app->GetRenderer()->RenderSprite(textureExtraData, m_x + nOffsetX, m_y + nOffsetY, STATES_COUNT, frame);
}

/*
==================
Button::HandleMouse

	Normally called when mouse over button AABB
==================
*/
bool Button::HandleMouse(const SDL_Event * const msg) {
	bool processed = false;

	switch (msg->type) {
		case SDL_EventType::SDL_MOUSEBUTTONUP:
			if (msg->button.button == SDL_BUTTON_LEFT && m_pressed) {
				m_pressed = false;

				if (m_pDialog) {
					m_pDialog->SendEvent(DialogEventType::BUTTON_CLICKED, this);
				}

				processed = true;
			}
			break;

		case SDL_EventType::SDL_MOUSEBUTTONDOWN:
			if (msg->button.button == SDL_BUTTON_LEFT) {
				m_pressed = true;
				processed = true;
			}
			break;

		default:
			break;
	}

	return processed;
}

/*
==================
Button::OnMouseLeave
==================
*/
void Button::OnMouseLeave() {
	m_pressed = false;
	Control::OnMouseLeave();
}
