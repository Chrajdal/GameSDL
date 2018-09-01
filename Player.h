#pragma once

#include "Graphics.h"
#include "Geometry.h"
#include "Utils.h"

class Player
{
public:
	Player(Image * img)
		:
		player_img(img),
		m_curr_state(PlayerState::idle)
	{
		pos.x = 0.0;
		pos.y = 0.0;

		vel.x = 0.0;
		vel.y = 0.0;
	}
	void Update()
	{
		switch (m_curr_state)
		{
		case PlayerState::idle:
			break;
		case PlayerState::walk:
			break;
		case PlayerState::jump:
			break;
		case PlayerState::attack:
			break;
		}
	}

	enum PlayerState
	{
		idle,
		walk,
		jump,
		attack
	};

	v2d pos;
	v2d vel;
	Image * player_img;
	PlayerState m_curr_state;
	Trect<int> m_bounding_box;
};