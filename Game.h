#pragma once

#include <bitset>
#include <algorithm>
#include <functional>
#include <thread>
#include <mutex>
#include <future>
#include <fstream>

#include "Camera.h"
#include "Graphics.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Utils.h"
#include "QuadTree.h"
#include "Geometry.h"
#include "Random.h"
#include "Player.h"

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

	Random rnd;
	CTimer timer;
};