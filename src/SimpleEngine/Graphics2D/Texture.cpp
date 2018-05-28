#include <png.h>
#include <SDL2/SDL.h>
#include "../include/Graphics2D/Texture.hpp"
#include "../include/SimpleEngineApp.hpp"
#include "../include/Graphics2D/Renderer.hpp"


constexpr size_t PNGSIGSIZE = 8;

/*
==================
TextureExtraData::~TextureExtraData
==================
*/
TextureExtraData::~TextureExtraData() {
	if (m_texture) {
		SDL_DestroyTexture(m_texture);
	}
}

static size_t g_readPos = 0;

/*
==================
ReadDataFunc
==================
*/
static void ReadDataFunc(png_structp pngPtr, png_bytep data, png_size_t length) {
	auto src = &static_cast<uint8_t*>(png_get_io_ptr(pngPtr))[g_readPos];
	memcpy(data, src, length);
	g_readPos += length;
}

/*
==================
PngImageLoader::LoadResource

	Code from lib SDL_Image
==================
*/
bool PngImageLoader::LoadResource(uint8_t * rawBuffer, size_t rawSize, std::shared_ptr<ResHandle> handle) {
	if (png_sig_cmp(rawBuffer, 0, PNGSIGSIZE)) {
		return false;		// not a png image
	}
	g_readPos = PNGSIGSIZE;

	bool result = true;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	Uint32 Rmask, Gmask, Bmask, Amask;
	SDL_Palette *palette;
	int row, i;
	int ckey = -1;
	png_color_16 *transv;
	std::shared_ptr<TextureExtraData> textureExtraData;

	/* Create the PNG loading context structure */
	auto png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
	if (png_ptr == nullptr) {
		result = false;
		goto done;
	}

	/* Allocate/initialize the memory for image information.  REQUIRED. */
	auto info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr) {
		result = false;
		goto done;
	}

	/* Set up the input control */
	png_set_read_fn(png_ptr, rawBuffer, ReadDataFunc);
	png_set_sig_bytes(png_ptr, PNGSIGSIZE);

	/* Read PNG header info */
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, nullptr, nullptr);

	/* tell libpng to strip 16 bit/color files down to 8 bits/color */
	png_set_strip_16(png_ptr);

	/* tell libpng to de-interlace (if the image is interlaced) */
	png_set_interlace_handling(png_ptr);

	/* Extract multiple pixels with bit depths of 1, 2, and 4 from a single
	* byte into separate bytes (useful for paletted and grayscale images).
	*/
	png_set_packing(png_ptr);

	/* scale greyscale values to the range 0..255 */
	if (color_type == PNG_COLOR_TYPE_GRAY) {
		png_set_expand(png_ptr);
	}

	/* For images with a single "transparent colour", set colour key;
	if more than one index has transparency, or if partially transparent
	entries exist, use full alpha channel */
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		int num_trans;
		Uint8 *trans;
		png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &transv);
		if (color_type == PNG_COLOR_TYPE_PALETTE) {
			/* Check if all tRNS entries are opaque except one */
			int j, t = -1;
			for (j = 0; j < num_trans; j++) {
				if (trans[j] == 0) {
					if (t >= 0) {
						break;
					}
					t = j;
				} else if (trans[j] != 255) {
					break;
				}
			}
			if (j == num_trans) {
				/* exactly one transparent index */
				ckey = t;
			} else {
				/* more than one transparent index, or translucency */
				png_set_expand(png_ptr);
			}
		} else {
			ckey = 0; /* actual value will be set later */
		}
	}

	if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
		png_set_gray_to_rgb(png_ptr);
	}

	png_read_update_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, nullptr, nullptr);

	/* Allocate the SDL surface to hold the image */
	Rmask = Gmask = Bmask = Amask = 0;
	const int num_channels = png_get_channels(png_ptr, info_ptr);
	if (num_channels >= 3) {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		Rmask = 0x000000FF;
		Gmask = 0x0000FF00;
		Bmask = 0x00FF0000;
		Amask = (num_channels == 4) ? 0xFF000000 : 0;
#else
		int s = (num_channels == 4) ? 0 : 8;
		Rmask = 0xFF000000 >> s;
		Gmask = 0x00FF0000 >> s;
		Bmask = 0x0000FF00 >> s;
		Amask = 0x000000FF >> s;
#endif
	}
	auto surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, bit_depth*num_channels, Rmask, Gmask, Bmask, Amask);
	if (surface == nullptr) {
		result = false;
		goto done;
	}

	if (ckey != -1) {
		if (color_type != PNG_COLOR_TYPE_PALETTE) {
			/* FIXME: Should these be truncated or shifted down? */
			ckey = SDL_MapRGB(surface->format, (Uint8)transv->red, (Uint8)transv->green, (Uint8)transv->blue);
		}
		SDL_SetColorKey(surface, SDL_TRUE, ckey);
	}

	/* Create the array of pointers to image data */
	auto row_pointers = new(std::nothrow) png_bytep[height];
	if (row_pointers == nullptr) {
		result = false;
		goto done;
	}
	for (row = 0; row < (int)height; row++) {
		row_pointers[row] = (png_bytep)(Uint8 *)surface->pixels + row*surface->pitch;
	}

	/* Read the entire image in one go */
	png_read_image(png_ptr, row_pointers);

	/* and we're done!  (png_read_end() can be omitted if no processing of
	* post-IDAT text/time/etc. is desired)
	* In some cases it can't read PNG's created by some popular programs (ACDSEE),
	* we do not want to process comments, so we omit png_read_end

	png_read_end(png_ptr, info_ptr);
	*/

	/* Load the palette, if any */
	palette = surface->format->palette;
	if (palette) {
		int png_num_palette;
		png_colorp png_palette;
		png_get_PLTE(png_ptr, info_ptr, &png_palette, &png_num_palette);
		if (color_type == PNG_COLOR_TYPE_GRAY) {
			palette->ncolors = 256;
			for (i = 0; i < 256; i++) {
				palette->colors[i].r = (Uint8)i;
				palette->colors[i].g = (Uint8)i;
				palette->colors[i].b = (Uint8)i;
			}
		} else if (png_num_palette > 0) {
			palette->ncolors = png_num_palette;
			for (i = 0; i<png_num_palette; ++i) {
				palette->colors[i].b = png_palette[i].blue;
				palette->colors[i].g = png_palette[i].green;
				palette->colors[i].r = png_palette[i].red;
			}
		}
	}

	textureExtraData = g_app->GetRenderer()->CreateTextureFromSurface(surface, width, height);
	if (!textureExtraData) {
		result = false;
		goto done;
	}

	handle->SetExtra(textureExtraData);

done:   /* Clean up and return */
	if (png_ptr) {
		png_destroy_read_struct(&png_ptr, info_ptr ? &info_ptr : (png_infopp)0, (png_infopp)0);
	}
	if (row_pointers) {
		delete[] row_pointers;
	}
	if (surface) {
		SDL_FreeSurface(surface);
	}

	return result;
}

