#include <iostream>
#include <thread>
#include <SDL.h>
#include "Utils.h"
#include <SDL_ttf.h>
#undef main

#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")
#pragma comment (lib, "SDL2_ttf.lib")

#include "Game.h"

int main(void)
{
	Game TheGame;

	while (TheGame.IsRunning() == true)
	{
		CTimer frame_timer;
		TheGame.Go();
		double frame_time = frame_timer.elapsed();

		//std::cout << "FPS: " << 1000 / (frame_time + 1e-6) << std::endl;
		if (frame_time < 1000/60.0)
		{
			double frame_wait = (1000 / 60.0) - frame_time;
			
			std::this_thread::sleep_for(std::chrono::milliseconds((unsigned)frame_wait));
		}
	}

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}