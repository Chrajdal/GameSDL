#pragma once

#include "Graphics.h"

class Camera
{
public:
	Camera(void)
	{
		m_x = 0;
		m_y = 0;
		
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
			SDL_Log("SDL_GetDesktopDisplayMode failed in Camera CONSTRUCTOR: %s", SDL_GetError());

		m_width = dm.w;
		m_height = dm.h;
	}

public:
	double m_x; // current position of the camera
	double m_y; // current position of the camera
private:
	int m_width;  // viewport size = Graphics::ScreenWidth
	int m_height; // viewport size = Graphics::ScreenHeight
};