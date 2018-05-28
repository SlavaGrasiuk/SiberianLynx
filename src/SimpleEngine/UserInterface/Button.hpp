#pragma once

#include "Control.hpp"
#include <memory>
#include <string>


class ResHandle;


class Button : public Control {
	std::shared_ptr<ResHandle> m_texture;
	bool m_pressed;

	static constexpr int STATES_COUNT = 3;

public:
	Button(Dialog* pDialog, const std::string &textureName);
	~Button() = default;

	virtual void Render(float deltaTimeMs) override;

	virtual bool HandleMouse(const SDL_Event * const msg) override;

	virtual void SetSize(int width, int height) override final {}		// simple image-based button not resizable
	
	virtual void OnMouseLeave() override;
};

