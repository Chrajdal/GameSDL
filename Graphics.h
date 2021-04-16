#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "SDL2_gfxPrimitives.h"

#include <vector>
#include <string>
#include <iostream>

#include "Color.h"
#include "Utils.h"

class Graphics;

class Image
{
public:
	Image(void);
	~Image(void);
	
	bool LoadData(Graphics & gfx, const std::string & file_name);
	SDL_Texture * GetData(void)const;
private:
	SDL_Texture * m_texture;
public:
	int m_width;
	int m_height;
};

class Graphics
{
public:
	Graphics(void);
	~Graphics(void);

	void BeginFrame(void);
	void EndFrame(void);

	void PutPixel(unsigned x, unsigned y, const Color & color);
	void DrawLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color & color);
	void DrawThickLine(unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color& color);

	void DrawCircle(int _x, int _y, int radius, const Color & c);
	void DrawRect(int rx, int ry, int w, int h, const Color & color);
	void DrawFilledCircle(int _x, int _y, int radius, const Color& c);
	void DrawFilledRect(int rx, int ry, int w, int h, const Color& color);
	void DrawCubicBezierCurve(const std::vector<Tpoint<float>> & points, const Color & color);

	void DrawImage(int x, int y, const Image & img);
	void DrawPartImage(int x, int y, int fromx, int fromy, int width, int height, const Image & img);

	inline unsigned width(void) { return ScreenWidth; }
	inline unsigned height(void){ return ScreenHeight; }	
public:
	SDL_Window * wnd;
	SDL_Renderer * ren;

	unsigned int ScreenWidth;
	unsigned int ScreenHeight;
};
