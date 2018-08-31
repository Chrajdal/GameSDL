#pragma once

#include <SDL.h>

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	bool PollEvent(void);
	bool KeyIsPressed(const SDL_Scancode & scan_code);
	bool KeyWasPressedOnce(const SDL_Keycode & scan_code);
};

