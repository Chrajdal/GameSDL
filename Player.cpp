#include "Player.h"

Player::Player(Image * img)
	:
	m_player_img(img),
	m_curr_state(PlayerState::idle)
{
	m_pos.x = 0.0;
	m_pos.y = 0.0;

	m_vel.x = 0.0;
	m_vel.y = 0.0;
}

Player::~Player(void)
{
	m_player_img = NULL;
}

void Player::Update(const QuadTree & terrain)
{
	// check collisions
	/*
	 -> a-j are tiles next to the player
	 -> .. is the player

	    ab
	   c..f
	   d..g
	   e..h
	    ij
	*/

	//if (m_vel.absolute() > 1e-3)
	{
		if (m_vel.y < 0)
		{
			if (GetTColl(terrain) == true)
			{
				m_vel.y = 0;
				m_pos.x += m_vel.x;
				m_pos.y = std::round(m_pos.y);
			}
		}
		if (m_vel.y > 0)
		{
			if (GetBColl(terrain) == true)
			{
				m_vel.y = 0;
				m_pos.x += m_vel.x;
				m_pos.y = std::round(m_pos.y);
				if (m_curr_state == PlayerState::jump)
					m_curr_state = PlayerState::idle;
			}
		}
		if (m_vel.x < 0)
		{
			if (GetLColl(terrain) == true)
			{
				m_vel.x = 0;
				m_pos.y += m_vel.y;
				m_pos.x = std::round(m_pos.x);
			}
		}
		if (m_vel.x > 0)
		{
			if (GetRColl(terrain) == true)
			{
				m_vel.x = 0;
				m_pos.y += m_vel.y;
				m_pos.x = std::round(m_pos.x);
			}
		}
	}

	// apply acceleration (gravity):
	v2d G(0, 0.1); // + value means down on screen
	ApplyForce(G);

	// apply friction
	m_vel *= 0.8;
		
	// apply velocity
	m_pos += m_vel;

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

void Player::ApplyForce(const v2d & force)
{
	m_vel += force;
}

bool Player::CheckCeilingCollision(const QuadTree & terrain, Graphics & gfx, Camera & cam) const
{
	Trect<double> ceiling_box = { m_pos.x , m_pos.y - 1 ,  m_pos.x + m_bbox.Width() , m_pos.y };

	std::cout << ceiling_box << "\n";

	bool collision = false;
	std::vector<const Node *> ceiling = terrain.range(ceiling_box);
	for (const auto & i : ceiling)
		if (i->m_tile != tile_type::air)
			collision = true;

	ceiling_box.m_upleft.m_x = ceiling_box.m_upleft.m_x * 16 - std::round(cam.m_pos.x);
	ceiling_box.m_upleft.m_y = ceiling_box.m_upleft.m_y * 16 - std::round(cam.m_pos.y);
	ceiling_box.m_downright.m_x = ceiling_box.m_downright.m_x * 16 - std::round(cam.m_pos.x);
	ceiling_box.m_downright.m_y = ceiling_box.m_downright.m_y * 16 - std::round(cam.m_pos.y);
	
	if (collision == false)
		gfx.DrawRect(
		(int)ceiling_box.m_upleft.m_x, (int)ceiling_box.m_upleft.m_y,
			(int)ceiling_box.Width(), (int)ceiling_box.Height(),
			Colors::Blue);
	else
		gfx.DrawRect(
		(int)ceiling_box.m_upleft.m_x, (int)ceiling_box.m_upleft.m_y,
			(int)ceiling_box.Width(), (int)ceiling_box.Height(),
			Colors::Cyan);
	return collision;
}

bool Player::CheckFloorCollision(const QuadTree & terrain, Graphics & gfx, Camera & cam) const
{
	Trect <double> floor_box = { m_pos.x, m_pos.y + m_bbox.Height(), m_pos.x + m_bbox.Width() , m_pos.y + m_bbox.Height() + 1 };

	std::cout << floor_box << "\n";

	bool collision = false;
	std::vector<const Node *> floor = terrain.range(floor_box);
	for (const auto & i : floor)
		if (i->m_tile != tile_type::air)
			collision = true;

	floor_box.m_upleft.m_x =    floor_box.m_upleft.m_x * 16    - std::round(cam.m_pos.x);
	floor_box.m_upleft.m_y =    floor_box.m_upleft.m_y * 16    - std::round(cam.m_pos.y);
	floor_box.m_downright.m_x = floor_box.m_downright.m_x * 16 - std::round(cam.m_pos.x);
	floor_box.m_downright.m_y = floor_box.m_downright.m_y * 16 - std::round(cam.m_pos.y);

	if(collision == false)
	gfx.DrawRect(
		(int)floor_box.m_upleft.m_x, (int)floor_box.m_upleft.m_y,
		(int)floor_box.Width(), (int)floor_box.Height(),
		Colors::Red);
	else
		gfx.DrawRect(
		(int)floor_box.m_upleft.m_x, (int)floor_box.m_upleft.m_y,
			(int)floor_box.Width(), (int)floor_box.Height(),
			Colors::Cyan);

	return false;
}

bool Player::CheckLeftWallCollision(const QuadTree & terrain, Graphics & gfx, Camera & cam) const
{
	Trect<double> left_wall_box = { m_pos.x - 1, m_pos.y - 1, m_pos.x , m_pos.y + m_bbox.Height() + 1 };

	std::cout << left_wall_box << "\n";

	bool collision = false;
	std::vector<const Node *> left_wall = terrain.range(left_wall_box);
	
	collision = std::find_if(left_wall.begin(), left_wall.end(), [](const Node * n) {return n->m_tile == tile_type::air; }) == left_wall.end();
	//for (const auto & i : left_wall)
	//	if (i->m_tile != tile_type::air)
	//		collision = true;

	left_wall_box.m_upleft.m_x =    left_wall_box.m_upleft.m_x * 16    - std::round(cam.m_pos.x);
	left_wall_box.m_upleft.m_y =    left_wall_box.m_upleft.m_y * 16    - std::round(cam.m_pos.y);
	left_wall_box.m_downright.m_x = left_wall_box.m_downright.m_x * 16 - std::round(cam.m_pos.x);
	left_wall_box.m_downright.m_y = left_wall_box.m_downright.m_y * 16 - std::round(cam.m_pos.y);

	if(collision == false)
	gfx.DrawRect(
		(int)left_wall_box.m_upleft.m_x, (int)left_wall_box.m_upleft.m_y,
		(int)left_wall_box.Width(), (int)left_wall_box.Height(),
		Colors::Green);
	else
		gfx.DrawRect(
		(int)left_wall_box.m_upleft.m_x, (int)left_wall_box.m_upleft.m_y,
			(int)left_wall_box.Width(), (int)left_wall_box.Height(),
			Colors::Cyan);
	return collision;
}

bool Player::CheckRightWallCollision(const QuadTree & terrain, Graphics & gfx, Camera & cam) const
{
	Trect<double> right_wall_box = { m_pos.x + m_bbox.Width(), m_pos.y, m_pos.x + m_bbox.Width() + 1, m_pos.y + m_bbox.Height() };

	std::cout << right_wall_box << "\n";

	bool collision = false;
	std::vector<const Node *> right_wall = terrain.range(right_wall_box);
	for (const auto & i : right_wall)
		if (i->m_tile != tile_type::air)
			collision = true;

	right_wall_box.m_upleft.m_x    = right_wall_box.m_upleft.m_x    * 16 - std::round(cam.m_pos.x);
	right_wall_box.m_upleft.m_y    = right_wall_box.m_upleft.m_y    * 16 - std::round(cam.m_pos.y);
	right_wall_box.m_downright.m_x = right_wall_box.m_downright.m_x * 16 - std::round(cam.m_pos.x);
	right_wall_box.m_downright.m_y = right_wall_box.m_downright.m_y * 16 - std::round(cam.m_pos.y);

	if(collision == false)
	gfx.DrawRect(
		(int)right_wall_box.m_upleft.m_x, (int)right_wall_box.m_upleft.m_y,
		(int)right_wall_box.Width(), (int)right_wall_box.Height(),
		Colors::Yellow);
	else
		gfx.DrawRect(
		(int)right_wall_box.m_upleft.m_x, (int)right_wall_box.m_upleft.m_y,
			(int)right_wall_box.Width(), (int)right_wall_box.Height(),
			Colors::Cyan);
	return collision;
}
