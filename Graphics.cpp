#include "Graphics.h"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Image::Image(void)
	: m_texture(nullptr),
	m_width(0),
	m_height(0)
{}
Image::~Image(void)
{
	if (m_texture != nullptr)
		SDL_DestroyTexture(m_texture);
}

bool Image::LoadData(Graphics & gfx, const std::string & file_name)
{
	//Get rid of preexisting texture
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_width = 0;
		m_height = 0;
	}

	//The final texture
	SDL_Texture * nTexture = nullptr;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(file_name.c_str());
	if (loadedSurface == nullptr)
		SDL_Log("IMG_Load failed: %s", SDL_GetError());
	else
	{
		//Create texture from surface pixels
		nTexture = SDL_CreateTextureFromSurface(gfx.ren, loadedSurface);
		if (nTexture == nullptr)
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
	return m_texture != nullptr;
}

SDL_Texture * Image::GetData(void)const
{
	return m_texture;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


Graphics::Graphics(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		SDL_Log("SDL_Init failed: %s", SDL_GetError());

	SDL_DisplayMode dm;
	if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());

	//ScreenHeight = dm.h;
	//ScreenWidth = dm.w;

	ScreenHeight = 512 + 256;
	ScreenWidth = 512 + 256;

	wnd = SDL_CreateWindow(
		"The Game",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		ScreenWidth, ScreenHeight,
		0);

	if (wnd == nullptr)
		SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());

	ren = SDL_CreateRenderer(wnd, 0, 0);
	if (ren == nullptr)
		SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());

	// Toggle Fullscreen
	//Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	//bool IsFullscreen = SDL_GetWindowFlags(wnd) & FullscreenFlag;
	//SDL_SetWindowFullscreen(wnd, IsFullscreen ? 0 : FullscreenFlag);
	//SDL_ShowCursor(SDL_ENABLE);
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

void Graphics::PutPixel(unsigned x, unsigned y, const Color & c)
{
	pixelRGBA(ren, x, y, c.GetR(), c.GetG(), c.GetB(), c.GetA());
}

void Graphics::DrawLine(unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color & c)
{
	lineRGBA(ren, x1, y1, x2, y2, c.GetR(), c.GetG(), c.GetB(), c.GetA());
}

//void Graphics::DrawThickLine(unsigned width, unsigned x1, unsigned y1, unsigned x2, unsigned y2, const Color& color)
//{
//
//}

void Graphics::DrawCircle(int _x, int _y, int radius, const Color & c)
{
	circleRGBA(ren, _x, _y, radius, c.GetR(), c.GetG(), c.GetB(), c.GetA());
}

void Graphics::DrawRect(int x, int y, int w, int h, const Color & color)
{
	rectangleRGBA(ren, x, y, x + w, y + h, color.GetR(), color.GetG(), color.GetB(), color.GetA());
}

void Graphics::DrawFilledCircle(int _x, int _y, int radius, const Color& c)
{
	filledCircleRGBA(ren, _x, _y, radius, c.GetR(), c.GetG(), c.GetB(), c.GetA());
}

void Graphics::DrawFilledRect(int x, int y, int w, int h, const Color& c)
{
	//rectangleRGBA(ren, x, y, x + w, y + h, c.GetR(), c.GetG(), c.GetB(), c.GetA());
	const Sint16 _x[4] = { x, x + w, x + w,x };
	const Sint16 _y[4] = { y, y, y + h,y + h };

	filledPolygonRGBA(ren, _x, _y, 4, c.GetR(), c.GetG(), c.GetB(), c.GetA());
}

void Graphics::DrawCubicBezierCurve(const std::vector<Tpoint<float>>& points, const Color& c)
{
	//De Casteljau's algorithm
	{
		if (points.size() < 4)
			return;
		auto pointLerp = [](const Tpoint<float>& a, const Tpoint<float>& b, const float& t) {
			auto px = std::lerp(a.m_x, b.m_x, t);
			auto py = std::lerp(a.m_y, b.m_y, t);
			return Tpoint<float>(px, py);
		};
		Tpoint<float> prev = points[0];
		for (float t = 0.0; t <= 1.0; t += .01)
		{
			auto p0 = points[0], p1 = points[1], p2 = points[2], p3 = points[3];
			auto p01 = pointLerp(p0, p1, t);
			auto p12 = pointLerp(p1, p2, t);
			auto p23 = pointLerp(p2, p3, t);
			auto p012 = pointLerp(p01, p12, t);
			auto p123 = pointLerp(p12, p23, t);
			auto p0123 = pointLerp(p012, p123, t);

			DrawLine(prev.m_x, prev.m_y, p0123.m_x, p0123.m_y, c);
			prev = p0123;
		}
		DrawLine(prev.m_x, prev.m_y, points.crbegin()->m_x, points.crbegin()->m_y, c);
	}

	return;

	//Bernstein polynomial implementation
	{
		double xu = 0.0, yu = 0.0, u = 0.0;
		int i = 0;
		std::pair<int, int> prev = { points.begin()->m_x, points.begin()->m_y };
		for (u = 0.0; u <= 1.0; u += 0.01)
		{
			xu = pow(1 - u, 3) * points[0].m_x + 3 * u * pow(1 - u, 2) * points[1].m_x + 3 * pow(u, 2) * (1 - u) * points[2].m_x
				+ pow(u, 3) * points[3].m_x;
			yu = pow(1 - u, 3) * points[0].m_y + 3 * u * pow(1 - u, 2) * points[1].m_y + 3 * pow(u, 2) * (1 - u) * points[2].m_y
				+ pow(u, 3) * points[3].m_y;
			//PutPixel((int)xu, (int)yu, c);
			DrawLine(xu, yu, prev.first, prev.second, c);
			prev = { xu, yu };
		}
		DrawLine(prev.first, prev.second, points.crbegin()->m_x, points.crbegin()->m_y, c);
	}
}

void Graphics::DrawImage(int x, int y, const Image & img)
{
	if (img.GetData() == nullptr)
		return;

	SDL_Rect m_pos;
	m_pos.x = x;
	m_pos.y = y;
	m_pos.w = img.m_width;
	m_pos.h = img.m_height;

	SDL_RenderCopy(ren, img.GetData(), nullptr, &m_pos);
}

void Graphics::DrawPartImage(int x, int y, int fromx, int fromy, int width, int height, const Image & img)
{
	if (img.GetData() == nullptr)
		return;

	SDL_Rect m_pos;
	m_pos.x = x;
	m_pos.y = y;
	m_pos.w = width;
	m_pos.h = height;

	SDL_Rect src;
	src.x = fromx;
	src.y = fromy;
	src.w = width;
	src.h = height;

	SDL_RenderCopy(ren, img.GetData(), &src, &m_pos);
}

//void Graphics::DrawFont(int x, int y, const Font & font)
//{
//	
//}

