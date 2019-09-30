#pragma once

#include "MovingEntity.h"
#include "Graphics.h"
#include "Geometry.h"
#include "QuadTree.h"
#include "Utils.h"
#include "Camera.h"
#include "Keyboard.h"

#include <iostream> // temporary

class Player : public MovingEntity // todo: class Player : protected MovingEntity
{
public:
	Player(Image * img);
	~Player();

	void Update(const QuadTree & terrain);
	void ApplyForce(const v2d & force);

	const float GRAVITY = 9.0f;
	const float MAX_VELOCITY = -10.0f;
	const float MAX_AIR_TIME = 1.2f;

	float timeInAir = 0.0f;
	float jumpImpulseTime = 0.2f;
	float jumpImpulseVel = -10.0f;
	float jumpAccel = -1.0f;


	void doJump(const QuadTree & terrain) {
		v2d acc (0.0, 0.0);
		v2d vel (0.0, 0.0);

		// Allow player to jump
		if (this->GetBColl(terrain)) {
			timeInAir = 0.0f;
			acc.y = 0.0;
			vel.y = 0.0;
		}

		// Handle vertical velocity and acceleration
		if (Keyboard::KeyIsPressed(Keyboard::Up)) {
			// First, jump up quickly..
			if (timeInAir < jumpImpulseTime) {
				vel.y = jumpImpulseVel;
			}
			// Then slowly go higher.. 
			else if (timeInAir < MAX_AIR_TIME) {
				acc.y = jumpAccel;
			}
			// Until finally falling
			else {
				acc.y = GRAVITY;
			}
		}
		else {
			// Prevent double jumps
			timeInAir = MAX_AIR_TIME;
			acc.y = GRAVITY;
		}

		m_vel.y = vel.y;
		m_acc.y = acc.y;
	}

	void update(float deltaTime) {
		
	}

	enum PlayerState
	{
		idle,
		walk,
		jump,
		attack
	};

	Image * m_player_img;
	PlayerState m_curr_state;

public: // only for now - make it private later
	bool CheckCeilingCollision  (const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
	bool CheckFloorCollision    (const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
	bool CheckLeftWallCollision (const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
	bool CheckRightWallCollision(const QuadTree & terrain, Graphics & gfx, Camera & cam)const;
};