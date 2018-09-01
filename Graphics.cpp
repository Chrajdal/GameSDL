#include "Graphics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Image::Image(void)
	: m_texture(NULL),
	m_width(0),
	m_height(0)
{}
Image::~Image(void)
{
	if (m_texture != NULL)
		SDL_DestroyTexture(m_texture);
}

bool Image::LoadData(Graphics & gfx, const std::string & file_name)
{
	//Get rid of preexisting texture
	if (m_texture != NULL)
	{
		SDL_DestroyTexture(m_texture);
		m_width = 0;
		m_height = 0;
	}

	//The final texture
	SDL_Texture * nTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(file_name.c_str());
	if (loadedSurface == NULL)
		SDL_Log("IMG_Load failed: %s", SDL_GetError());
	else
	{
		//Create texture from surface pixels
		nTexture = SDL_CreateTextureFromSurface(gfx.ren, loadedSurface);
		if (nTexture == NULL)
			SDL_Log("SDL_CreateTextureFromSurface failed: %s", SDL_GetError());
		else
		{
			//Get image dimensions
			m_width = loadedSurface->w;
			m_height = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	m_texture = nTexture;
	return m_texture != NULL;
}

SDL_Texture * Image::GetData(void)const
{
	return m_texture;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


Graphics::Graphics(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != NULL)
		SDL_Log("SDL_Init failed: %s", SDL_GetError());

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

	ScreenHeight = dm.h;
	ScreenWidth = dm.w;

	//ScreenHeight = 512 + 256;
	//ScreenWidth = 512 + 256;

	wnd = SDL_CreateWindow(
		"The Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		ScreenWidth, ScreenHeight,
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
	SDL_ShowCursor(SDL_ENABLE);
}

Graphics::~Graphics(void)
{
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(wnd);
}

void Graphics::BeginFrame(void)
{
	SDL_SetRenderDrawColor(ren, 0x80, 0xc8, 0xff, 255);
	SDL_RenderClear(ren);
}

void Graphics::EndFrame(void)
{
	SDL_RenderPresent(ren);
}

void Graphics::PutPixel(unsigned x, unsigned y, const Color & color)
{
	SDL_SetRenderDrawColor(ren, color.GetR(), color.GetG(), color.GetB(), color.GetA());
	SDL_RenderDrawPoint(ren, x, y);
}

void Graphics::DrawLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color & color)
{
	SDL_SetRenderDrawColor(ren, color.GetR(), color.GetG(), color.GetB(), color.GetA());
	SDL_RenderDrawLine(ren, x1, y1, x2, y2);
}

void Graphics::DrawCircle(int _x, int _y, int radius, const Color & c)
{
	int x = radius - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (radius << 1);
	while (x >= y)
	{
		PutPixel(_x + x, _y + y, c);
		PutPixel(_x + y, _y + x, c);
		PutPixel(_x - y, _y + x, c);
		PutPixel(_x - x, _y + y, c);
		PutPixel(_x - x, _y - y, c);
		PutPixel(_x - y, _y - x, c);
		PutPixel(_x + y, _y - x, c);
		PutPixel(_x + x, _y - y, c);
		if (err <= 0)
		{
			y++;
			err += dy;
			dy += 2;
		}
		if (err > 0)
		{
			x--;
			dx += 2;
			err += (-radius << 1) + dx;
		}
	}
}

void Graphics::DrawImage(int x, int y, const Image & img)
{
	if (img.GetData() == NULL)
		return;

	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	pos.w = img.m_width;
	pos.h = img.m_height;

	SDL_RenderCopy(ren, img.GetData(), NULL, &pos);
}

void Graphics::DrawPartImage(int x, int y, int fromx, int fromy, int width, int height, const Image & img)
{
	if (img.GetData() == NULL)
		return;

	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	pos.w = width;
	pos.h = height;

	SDL_Rect src;
	src.x = fromx;
	src.y = fromy;
	src.w = width;
	src.h = height;

	SDL_RenderCopy(ren, img.GetData(), &src, &pos);
}