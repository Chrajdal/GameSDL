#pragma once

#include <SDL.h>

#include "Color.h"

namespace gfx
{
	static unsigned int ScreenHeight;
	static unsigned int ScreenWidth;
}

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

	void BeginFrame(void);
	void EndFrame(void);

	void PutPixel(unsigned x, unsigned y, const Color & color);
	void DrawLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color & color);

private:
	SDL_Window * wnd;
	SDL_Renderer * ren;
};