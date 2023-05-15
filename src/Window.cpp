#include "Window.hpp"
#include "Log.hpp"
#include <SDL_ttf.h>


Window::Window() : SDLWindow(nullptr), width(WINDOW_WIDTH), height(WINDOW_HEIGHT)
{
}

bool Window::initialize()
{
	int sdlInitResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlInitResult != 0) {
		Log::error(LogCategory::Video, "Unable to initialize SDL");
		return false;
	}
	SDLWindow = SDL_CreateWindow("Widow", 100, 100, width, height, SDL_WINDOW_OPENGL); // 0 is flag we will use later
	if (!SDLWindow)
	{
		Log::error(LogCategory::System, "Failed to create window");
		return false;
	}

	if (TTF_Init() < 0)
	{
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
		return false;
	}

	return true;
}

void Window::close()
{
	TTF_Quit();
	SDL_DestroyWindow(SDLWindow);
}