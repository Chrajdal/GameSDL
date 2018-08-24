#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <iostream>

#include "Color.h"

//class Surface
//{
//public:
//	Surface(void)
//		: m_data(NULL)
//	{}
//	~Surface(void)
//	{
//		if (m_data != NULL)
//			SDL_FreeSurface(m_data);
//	}
//
//	void LoadData(const std::string & file_name)
//	{
//		m_data = IMG_Load(file_name.c_str());
//		if (m_data == NULL)
//		{
//			std::cout << "IMG_Load failed: " << std::endl;
//			SDL_Log("IMG_Load failed: %s", SDL_GetError());
//		}
//	}
//
//	SDL_Surface * GetData(void)const
//	{
//		return m_data;
//	}
//
//	SDL_Surface * m_data;
//};

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

	void DrawImage(int x, int y, SDL_Surface * srf);

	unsigned int ScreenHeight;
	unsigned int ScreenWidth;
private:
	SDL_Window * wnd;
	SDL_Renderer * ren;
};