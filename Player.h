#pragma once

#include "Entity.h"
#include "Graphics.h"
#include "Geometry.h"
#include "QuadTree.h"
#include "Utils.h"

class Player : public Entity // todo: class Player : private Entity
{
public:
	Player(Image * img);
	~Player();

	void Update(const QuadTree & terrain);
	void ApplyForce(const v2d & force);

	enum PlayerState
	{
		idle,
		walk,
		jump,
		attack
	};

	Image * m_player_img;
	PlayerState m_curr_state;
	Trect<int> m_bbox;

private:
	bool CheckCeilingCollision(const QuadTree & terrain)const;
	bool CheckFloorCollision(const QuadTree & terrain)const;
	bool CheckLeftWallCollision(const QuadTree & terrain)const;
	bool CheckRightWallCollision(const QuadTree & terrain)const;
};