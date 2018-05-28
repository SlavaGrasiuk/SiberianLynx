#include <SDL2/SDL.h>
#include "../include/Graphics2D/Renderer.hpp"
#include "../include/SimpleEngineApp.hpp"


/*
==================
Renderer2D::~Renderer2D
==================
*/
Renderer2D::~Renderer2D() {
	if (m_renderer) {
		SDL_DestroyRenderer(m_renderer);
	}
}

/*
==================
Renderer2D::Init
==================
*/
bool Renderer2D::Init(const bool vsync, const bool hwaccel) {
	const uint32_t flags = (vsync ? SDL_RENDERER_PRESENTVSYNC : 0) | (hwaccel ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE);

	m_renderer = SDL_CreateRenderer(g_app->GetWindow(), -1, flags);
	if (m_renderer == nullptr) {
		return false;
	}
	
	return true;
}

/*
==================
Renderer2D::RenderSprite
==================
*/
void Renderer2D::RenderSprite(std::shared_ptr<TextureExtraData> texture, const size_t x, const size_t y, const int frameCount, const int frameNumber) {
	SDL_Rect src, dst;
	const auto w = texture->GetWidth();
	const auto h = texture->GetHeigth();
	const auto frameW = w / frameCount;

	dst.x = x;
	dst.y = y;
	dst.w = frameW;
	dst.h = h;

	src.x = frameW * frameNumber;
	src.y = 0;
	src.w = frameW;
	src.h = h;

	SDL_RenderCopy(m_renderer, texture->GetTexture(), &src, &dst);
}

/*
==================
Renderer2D::CreateTextureFromSurface
==================
*/
std::shared_ptr<TextureExtraData> Renderer2D::CreateTextureFromSurface(SDL_Surface * surface, size_t width, size_t heigth) {
	auto texture = SDL_CreateTextureFromSurface(m_renderer, surface);
	if (texture == nullptr) {
		return std::shared_ptr<TextureExtraData>();
	}

	return make_shared<TextureExtraData>(texture, width, heigth);
}

/*
==================
Renderer2D::EnableAlphaBlending
==================
*/
bool Renderer2D::EnableAlphaBlending(std::shared_ptr<TextureExtraData> texture) {
	int result = SDL_SetTextureBlendMode(texture->GetTexture(), SDL_BlendMode::SDL_BLENDMODE_BLEND);
	return (result ? false : true);
}

bool Renderer2D::DisableAlphaBlending(std::shared_ptr<TextureExtraData> texture) {
	int result = SDL_SetTextureBlendMode(texture->GetTexture(), SDL_BlendMode::SDL_BLENDMODE_NONE);
	return (result ? false : true);
}

/*
==================
Renderer2D::PresentScene
==================
*/
void Renderer2D::PresentScene() {
	SDL_RenderPresent(m_renderer);
}
