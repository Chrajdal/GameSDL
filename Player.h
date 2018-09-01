#pragma once

#include "Moveable.h"
#include "Graphics.h"
#include "Geometry.h"
#include "QuadTree.h"
#include "Utils.h"


class Player : public Moveable // todo: class Player : private Moveable
{
public:
	Player(Image * img);
	~Player();

	void Update(const QuadTree & terrain);

	enum PlayerState
	{
		idle,
		walk,
		jump,
		attack
	};

	//v2d m_pos;
	//v2d m_vel;
	Image * m_player_img;
	PlayerState m_curr_state;
	Trect<int> m_bounding_box;
};