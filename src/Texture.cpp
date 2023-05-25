#include "Texture.hpp"

#include <SDL_image.h>
#include <sstream>

#include "Log.hpp"


Texture::Texture() : textureID(0), filename(""), width(0), height(0), SDLTexture(nullptr)
{
}

Texture::~Texture()
{
}

void Texture::unload()
{
	if (SDLTexture)
	{
		SDL_DestroyTexture(SDLTexture);
	}
	else
	{
		glDeleteTextures(1, &textureID);
	}
}

bool Texture::loadOGL(RendererOGL& renderer, const string& filenameP)
{
	filename = filenameP;

	// Load from file
	SDL_Surface* surf = SDL_LoadBMP(filename.c_str());
	if (!surf)
	{
		Log::error(LogCategory::Application, "Failed to load texture file " + filename);
		return false;
	}

	width = surf->w;
	height = surf->h;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf->pixels);
	


	Log::info("Loaded texture " + filename);
	// Enable bilinear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glActiveTexture(GL_TEXTURE0);
	SDL_FreeSurface(surf);
	
	return true;
}

void Texture::updateInfo(int& widthOut, int& heightOut)
{
	widthOut = width;
	heightOut = height;
}

void Texture::setActive() const
{
	//glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::createFromSurface(SDL_Surface* surface)
{
	width = surface->w;
	height = surface->h;

	// Generate a GL texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}