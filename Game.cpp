#include "Game.h"


std::vector<Tpoint<float>> points = {
	{10.0f, 10.f},
	{600.f, 10.f},
	{10.f, 600.f},
	{600.f,600.f}
};

Game::Game()
{
	m_IsRunning = true;
}

Game::~Game(void)
{

}

void Game::Go()
{
	gfx.BeginFrame();

	if (m_IsRunning)
	{
		HandleInput();
	}
	if (m_IsRunning)
	{
		UpdateModel();
	}
	if (m_IsRunning)
	{
		ComposeFrame();
	}

	gfx.EndFrame();
}

void Game::HandleInput()
{
	SDL_Event e;
	SDL_PollEvent(&e);

	// keyboard code
	if (kbd.KeyIsPressed(SDL_SCANCODE_ESCAPE))
	{
		m_IsRunning = false;
		return;
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_DELETE))
	{
	}

	double step = 2e-1; // will be changed to player.m_vel;
	if (kbd.KeyIsPressed(SDL_SCANCODE_SPACE))
	{		
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_LEFT) || kbd.KeyIsPressed(SDL_SCANCODE_A))
	{
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_RIGHT) || kbd.KeyIsPressed(SDL_SCANCODE_D))
	{
	}

	if (kbd.KeyIsPressed(SDL_SCANCODE_1))
	{
	}
	if (kbd.KeyIsPressed(SDL_SCANCODE_2))
	{
	}
	
	//mouse.SetMouseState(e);
	// mouse code
	{
		int x = mouse.GetX();
		int y = mouse.GetY();
	}

	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
	//while (SDL_PollEvent(&e));
}

void Game::UpdateModel()
{

}

void Game::ComposeFrame()
{
	gfx.DrawFilledRect(50, 50, 150, 100, Colors::Cyan);

	gfx.DrawLine(points[0].m_x, points[0].m_y, points[1].m_x, points[1].m_y, Colors::LightGray);
	gfx.DrawLine(points[1].m_x, points[1].m_y, points[3].m_x, points[3].m_y, Colors::LightGray);
	gfx.DrawLine(points[2].m_x, points[2].m_y, points[3].m_x, points[3].m_y, Colors::LightGray);

	gfx.DrawCubicBezierCurve(points, Colors::MakeRGB(0xdd, 0x1a, 0x7d));

	for (auto& i : points)
	{
		gfx.DrawCircle(i.m_x, i.m_y, 5, Colors::Blue);

		i.m_x += rnd.NextFloat(-1.0f, 1.0f) / 1.e-2f;
		i.m_y += rnd.NextFloat(-1.0f, 1.0f) / 1.e-2f;
		if (i.m_x < 0)
			i.m_x = 0;
		if (i.m_x >= gfx.ScreenWidth)
			i.m_x = gfx.ScreenWidth - 1;
		if (i.m_y < 0)
			i.m_y = 0;
		if (i.m_y >= gfx.ScreenHeight)
			i.m_y = gfx.ScreenHeight - 1;
	}

}
