#include <iostream>
#include <thread>
#include <SDL.h>
#include "Utils.h"
#undef main

#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")

#include "Game.h"

int main(void)
{
	Game TheGame;
	while (TheGame.IsRunning() == true)
	{
		TheGame.Go();
	}
	
	IMG_Quit();
	SDL_Quit();

	return 0;
}
