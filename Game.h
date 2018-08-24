#pragma once

#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Utils.h"

class Game
{
public:
	Game();
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	~Game(void);
	
	void Go();
	inline bool IsRunning(void) const { return m_IsRunning; }
private:
	void HandleInput();
	void UpdateModel();
	void ComposeFrame();

private:
	Graphics gfx;
	Mouse mouse;
	Keyboard kbd;
	bool m_IsRunning;

	Surface img;
	//CTimer timer;
};