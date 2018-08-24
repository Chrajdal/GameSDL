#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>

#include "Color.h"
#include "Image.h"

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

	void BeginFrame(void);
	void EndFrame(void);

	void PutPixel(unsigned x, unsigned y, const Color & color);
	void DrawLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color & color);
	void DrawCircle(int _x, int _y, int radius, const Color & c);

	void DrawImage(int x, int y, const Image & img);
	void DrawPartImage(int x, int y, int fromx, int fromy, int width, int height, const Image & img);

	inline unsigned width(void) { return ScreenWidth; }
	inline unsigned height(void){ return ScreenHeight; }	
private:
	SDL_Window * wnd;
	SDL_Renderer * ren;

	unsigned int ScreenWidth;
	unsigned int ScreenHeight;
};
