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
	const Uint8 * keystate = SDL_GetKeyboardState(NULL);
	//continuous-response keys
	if (keystate != NULL)
		return keystate[scan_code];
	return false;
}

bool Keyboard::KeyWasPressedOnce(const SDL_Keycode & key_code)
{
	const Uint8 * keystate = SDL_GetKeyboardState(NULL);
	// non-continuous response keys
	if (keystate != NULL)
		return keystate[key_code];
	return false;
}