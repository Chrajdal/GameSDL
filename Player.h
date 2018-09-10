#pragma once

#include "MovingEntity.h"
#include "Graphics.h"
#include "Geometry.h"
#include "QuadTree.h"
#include "Utils.h"
#include "Camera.h"

#include <iostream> // temporary

class Player : public MovingEntity // todo: class Player : protected MovingEntity
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
	//Trect<int> m_bbox;

public: // only for now - make it private later
	bool CheckCeilingCollision  (const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
	bool CheckFloorCollision    (const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
	bool CheckLeftWallCollision (const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
	bool CheckRightWallCollision(const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
};