#include "Game.h"

#include <iostream>

Game::Game()
{
	m_IsRunning = true;
	gfx.BeginFrame();
	gfx.EndFrame();
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

}
