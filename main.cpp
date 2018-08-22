#include <iostream>
#include <SDL.h>

#undef main

#pragma comment (lib, "SDL2.lib")
#pragma comment (lib, "SDL2main.lib")
#pragma comment (lib, "SDL2_image.lib")

int main(void)
{
	std::cout << "Hello world" << std::endl;

	system("PAUSE");
	return 0;
}