#include <iostream>
#include <SDL.h>

#undef main

#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")

namespace gfx
{
	static unsigned int ScreenHeight;
	static unsigned int ScreenWidth;
}

class Color
{
public:
	unsigned int dword;
public:
	constexpr Color() : dword() {}
	constexpr Color(const Color& col)
		:
		dword(col.dword)
	{}
	constexpr Color(unsigned int dw)
		:
		dword(dw)
	{}
	constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b)
		:
		dword((x << 24u) | (r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(unsigned char r, unsigned char g, unsigned char b)
		:
		dword((r << 16u) | (g << 8u) | b)
	{}
	constexpr Color(Color col, unsigned char x)
		:
		Color((x << 24u) | col.dword)
	{}
	Color& operator =(Color color)
	{
		dword = color.dword;
		return *this;
	}
	constexpr unsigned char GetX() const
	{
		return dword >> 24u;
	}
	constexpr unsigned char GetA() const
	{
		return GetX();
	}
	constexpr unsigned char GetR() const
	{
		return (dword >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const
	{
		return (dword >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const
	{
		return dword & 0xFFu;
	}
	void SetX(unsigned char x)
	{
		dword = (dword & 0xFFFFFFu) | (x << 24u);
	}
	void SetA(unsigned char a)
	{
		SetX(a);
	}
	void SetR(unsigned char r)
	{
		dword = (dword & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG(unsigned char g)
	{
		dword = (dword & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB(unsigned char b)
	{
		dword = (dword & 0xFFFFFF00u) | b;
	}
};

namespace Colors
{
	static constexpr Color MakeRGB(unsigned char r, unsigned char g, unsigned char b)
	{
		return (r << 16) | (g << 8) | b;
	}
	static constexpr Color White = MakeRGB(255u, 255u, 255u);
	static constexpr Color Black = MakeRGB(0u, 0u, 0u);
	static constexpr Color Gray = MakeRGB(0x80u, 0x80u, 0x80u);
	static constexpr Color LightGray = MakeRGB(0xD3u, 0xD3u, 0xD3u);
	static constexpr Color Red = MakeRGB(255u, 0u, 0u);
	static constexpr Color Green = MakeRGB(0u, 255u, 0u);
	static constexpr Color Blue = MakeRGB(0u, 0u, 255u);
	static constexpr Color Yellow = MakeRGB(255u, 255u, 0u);
	static constexpr Color Cyan = MakeRGB(0u, 255u, 255u);
	static constexpr Color Magenta = MakeRGB(255u, 0u, 255u);
}

class Graphics
{
public:
	

	Graphics(void)
	{
		if (SDL_Init(SDL_INIT_VIDEO) != NULL)
			SDL_Log("SDL_Init failed: %s", SDL_GetError());

		SDL_DisplayMode dm;
		if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
			SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

		gfx::ScreenHeight= dm.h;
		gfx::ScreenWidth = dm.w;

		wnd = SDL_CreateWindow(
			"The Game",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			gfx::ScreenWidth, gfx::ScreenHeight,
			0);

		if (wnd == NULL)
			SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());

		ren = SDL_CreateRenderer(wnd, 0, 0);
		if (ren == NULL)
			SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());

		// Toggle Fullscreen
		Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
		bool IsFullscreen = SDL_GetWindowFlags(wnd) & FullscreenFlag;
		SDL_SetWindowFullscreen(wnd, IsFullscreen ? 0 : FullscreenFlag);
		SDL_ShowCursor(IsFullscreen);
	}

	void BeginFrame(void)
	{
		Color c = Colors::Black;
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		SDL_RenderClear(ren);
	}

	void EndFrame(void)
	{
		SDL_RenderPresent(ren);
	}

	void PutPixel(unsigned x, unsigned y, const Color & color)
	{
		SDL_SetRenderDrawColor(ren, color.GetR(), color.GetG(), color.GetB(), color.GetA());
		SDL_RenderDrawPoint(ren, x, y);
	}

	void DrawLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color & color)
	{
		SDL_SetRenderDrawColor(ren, color.GetR(), color.GetG(), color.GetB(), color.GetA());
		SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	}

	~Graphics(void)
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(wnd);
	}

	SDL_Window * wnd;
	SDL_Renderer * ren;
};

class Game
{
public:
	Game() {}
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	~Game(void) {}
	void Go()
	{
		HandleInput();
		ComposeFrame();
		UpdateModel();
	}
private:
	void HandleInput() {}
	void ComposeFrame(){}
	void UpdateModel() {}

private:
	Graphics gfx;
	//CTimer timer;
};
class Mouse
{

};

class Keyboard
{

};

int main(void)
{
	Graphics gfx;

	bool quit = false;
	SDL_Event e;
	while (quit == false)
	{
		gfx.BeginFrame();

		gfx.PutPixel(500, 500, Colors::White);
		gfx.DrawLine(501, 501, 700, 700, Colors::Blue);
		int x = 0, y = 0;
		SDL_GetMouseState(&x, &y);
		//if (SDL_GetMouseState(&x, &y & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			gfx.DrawLine(501, 501, x, y, Colors::MakeRGB((1-y) % 255, y % 255, 123));
		}
		

		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE: quit = true; break;
			}
		}

		gfx.EndFrame();
	}

	//system("PAUSE");
	return 0;
}