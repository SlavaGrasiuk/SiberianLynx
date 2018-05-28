#pragma once

#include "../Interfaces.hpp"
#include "../Resource/ResourceManager.hpp"


struct SDL_Texture;

/*
===============================================================================

	Holds texture info

===============================================================================
*/
class TextureExtraData : public IResourceExtraData {
	SDL_Texture *m_texture;
	size_t m_width, m_heigth;

public:
	TextureExtraData(SDL_Texture *texture, size_t width, size_t heigth) : m_texture(texture), m_width(width), m_heigth(heigth) {}
	~TextureExtraData();
	virtual std::string ToString() const override {
		return "TextureExtraData";
	}
	auto GetTexture() const {
		return m_texture;
	}
	auto GetWidth() const {
		return m_width;
	}
	auto GetHeigth() const {
		return m_heigth;
	}
};

/*
===============================================================================

	Loader for PNG images

===============================================================================
*/
class PngImageLoader : public IResourceLoader {

public:
	PngImageLoader() = default;

	virtual const char *GetPattern() const override {
		return "*.png";
	}

	virtual bool UseRawFile() const override {
		return false;
	}

	virtual bool DiscardRawBufferAfterLoad() const override {
		return true;
	}

	virtual size_t GetLoadedResourceSize(uint8_t *rawBuffer, size_t rawSize) override {
		return 0;	// This will keep the resource manager from allocating memory for the texture, so SDL2 can manage it on it's own.
	}

	virtual bool LoadResource(uint8_t *rawBuffer, size_t rawSize, std::shared_ptr<ResHandle> handle) override;
};

