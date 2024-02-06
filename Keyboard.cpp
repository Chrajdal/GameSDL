#include "Keyboard.h"

Keyboard::Keyboard() {}
Keyboard::~Keyboard() {}

bool Keyboard::PollEvent(void)
{
	SDL_Event e;
	if (SDL_PollEvent(&e))
		return true;
	return false;
}

bool Keyboard::KeyIsPressed(const SDL_Scancode & scan_code)
{
	const Uint8 * keystate = SDL_GetKeyboardState(nullptr);
	//continuous-response keys
	if (keystate != nullptr)
		return keystate[scan_code];
	return false;	
}

bool Keyboard::KeyWasPressedOnce(const SDL_Keycode & key_code)
{
	const Uint8 * keystate = SDL_GetKeyboardState(nullptr);
	// non-continuous response keys
	if (keystate != nullptr)
	{
		if (keystate[key_code])
		{
			SDL_PumpEvents();
			return true;
		}
	}
	return false;
}