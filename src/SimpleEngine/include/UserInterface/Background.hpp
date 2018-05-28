#pragma once

#include "..\Interfaces.hpp"


class Background : public IScreenElement {
	std::shared_ptr<ResHandle> m_texture;

public:
	Background(const std::string &textureName);
	~Background() = default;

	virtual void OnRender(const float deltaTimeMs) override;
	virtual void OnUpdate(const float deltaTimeMs) override {}

	virtual int GetZOrder() const override final {
		return INFINITY_FAR;
	}
	virtual void SetZOrder(const int zOrder) override final {}

	virtual bool IsVisible() const override final {
		return true;		// background may be used for clearing frame buffer, so it always visible
	}
	virtual void SetVisible(bool visible) override final {}

	virtual bool OnMsgProc(const SDL_Event * const msg) override final {
		return false;
	}
};
