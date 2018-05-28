#pragma once

#include "Texture.hpp"

struct SDL_Renderer;
struct SDL_Surface;

/*
===============================================================================

	Draws sprites in frame buffer and texture factory.

===============================================================================
*/
class Renderer2D {
	SDL_Renderer *m_renderer;

public:
	Renderer2D() : m_renderer(nullptr) {}
	~Renderer2D();

	bool Init(const bool vsync, const bool hwaccel);

	void RenderSprite(std::shared_ptr<TextureExtraData> texture, const size_t x, const size_t y, const int frameCount, const int frameNumber);

	std::shared_ptr<TextureExtraData> CreateTextureFromSurface(SDL_Surface *surface, size_t width, size_t heigth);

	bool EnableAlphaBlending(std::shared_ptr<TextureExtraData> texture);

	bool DisableAlphaBlending(std::shared_ptr<TextureExtraData> texture);

	void PresentScene();
};
