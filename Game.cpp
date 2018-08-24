#include "Game.h"

Game::Game()
{
	m_IsRunning = true;
	gfx.BeginFrame();
	gfx.EndFrame();


	img.LoadData("Main Character.png");
}

Game::~Game(void)
{
	
}

void Game::Go()
{
	gfx.BeginFrame();

	HandleInput();
	UpdateModel();
	ComposeFrame();

	gfx.EndFrame();
}

void Game::HandleInput()
{
	SDL_Event e;
	if (SDL_PollEvent(&e))
	{
		switch(e.type)
		{
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym)
			{
			case SDLK_ESCAPE: m_IsRunning = false; break;
			}
		break;
		}
	}
}

void Game::UpdateModel()
{

}

void Game::ComposeFrame()
{
	gfx.DrawCircle(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2, 50, Colors::MakeRGB(123, 231, 132));

	gfx.DrawImage(gfx.ScreenWidth / 2, gfx.ScreenHeight / 2, img.GetData());
}
