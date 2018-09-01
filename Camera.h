#pragma once

#include "Graphics.h"
#include "Geometry.h"

class Camera
{
public:
	Camera(void)
	{
		m_pos.x = 0;
		m_pos.y = 0;
		
		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
			SDL_Log("SDL_GetDesktopDisplayMode failed in Camera CONSTRUCTOR: %s", SDL_GetError());

		m_width = dm.w;
		m_height = dm.h;
	}

public:
	v2d m_pos; // current position of the camera
private:
	int m_width;  // viewport size = Graphics::ScreenWidth
	int m_height; // viewport size = Graphics::ScreenHeight
};