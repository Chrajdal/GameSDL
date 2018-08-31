#pragma once

#include <SDL.h>
#include <array>

class Mouse
{
public:
	Mouse();
	~Mouse();

	void SetMouseState(SDL_Event & event)
	{
		if (event.type == SDL_MOUSEBUTTONDOWN)
			MouseStates[event.button.button - 1] = true;
		if (event.type == SDL_MOUSEBUTTONUP)
			MouseStates[event.button.button - 1] = false;

		SDL_GetMouseState(&x, &y);
	}

	inline int GetX(void) const
	{
		return x;
	}

	inline int GetY(void) const
	{
		return y;
	}

	inline auto GetPos(void) const
	{
		std::make_pair(x, y);
	}

	inline bool LeftIsPressed(void)const
	{
		return MouseStates[mouse_buttons::Left];
	}

	inline bool RightIsPressed(void)const
	{
		return MouseStates[mouse_buttons::Right];
	}

private:
	std::array<bool,3> MouseStates;
	int x, y;

	enum mouse_buttons
	{
		Left,Center,Right
	};
};
