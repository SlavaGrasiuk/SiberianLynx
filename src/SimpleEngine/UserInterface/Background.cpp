#include "../include/UserInterface/Background.hpp"
#include "../include/Graphics2D/Renderer.hpp"
#include "../include/SimpleEngineApp.hpp"
#include <memory>


/*
==================
Background::Background
==================
*/
Background::Background(const std::string &textureName) : m_texture(g_app->GetResourceMan()->GetHandle(textureName)) {}

/*
==================
Background::OnRender
==================
*/
void Background::OnRender(const float deltaTimeMs) {
	if (m_texture) {
		auto textureExtraData = std::static_pointer_cast<TextureExtraData>(m_texture->GetExtra());

		g_app->GetRenderer()->DisableAlphaBlending(textureExtraData);
		g_app->GetRenderer()->RenderSprite(textureExtraData, 0, 0, 1, 0);
	}	// maybe render error-texture in case if user texture was not opened?
}
