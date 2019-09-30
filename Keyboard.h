#pragma once

#include <SDL.h>

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool PollEvent(void);
	static bool KeyIsPressed(const SDL_Scancode& scan_code);
	static bool KeyWasPressedOnce(const SDL_Keycode& scan_code);

	static const SDL_Scancode Up = SDL_SCANCODE_SPACE;
	static const SDL_Scancode Down = SDL_SCANCODE_S;
	static const SDL_Scancode Left = SDL_SCANCODE_A;
	static const SDL_Scancode Right = SDL_SCANCODE_D;
};

