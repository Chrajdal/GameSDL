#include "MovingEntity.h"



MovingEntity::MovingEntity()
{
}


MovingEntity::~MovingEntity()
{
}



bool MovingEntity::GetTColl(const QuadTree & terrain)
{
	std::cout << GetTRect() << std::endl;
	std::vector<const Node *> ceiling = terrain.range(GetTRect());
	return std::find_if(
		ceiling.begin(),
		ceiling.end(),
		[](const Node * n) {return n->m_tile == tile_type::air; }) == ceiling.end();
}

bool MovingEntity::GetBColl(const QuadTree & terrain)
{
	std::vector<const Node *> floor = terrain.range(GetBRect());
	return std::find_if(
		floor.begin(),
		floor.end(),
		[](const Node * n) {return n->m_tile == tile_type::air; }) == floor.end();
}

bool MovingEntity::GetLColl(const QuadTree & terrain)
{
	std::vector<const Node *> left_wall = terrain.range(GetLRect());
	return std::find_if(
		left_wall.begin(),
		left_wall.end(),
		[](const Node * n) {return n->m_tile == tile_type::air; }) == left_wall.end();
}

bool MovingEntity::GetRColl(const QuadTree & terrain)
{
	std::vector<const Node *> right_wall = terrain.range(GetRRect());
	return std::find_if(
		right_wall.begin(),
		right_wall.end(),
		[](const Node * n) {return n->m_tile == tile_type::air; }) == right_wall.end();
}

Trect<double> MovingEntity::GetTRect(void)
{
	return { std::floor(m_pos.x), m_pos.y - 1,  std::ceil(m_pos.x + m_bbox.Width()), m_pos.y };
}
Trect<double> MovingEntity::GetBRect(void)
{
	return { std::floor(m_pos.x), m_pos.y + m_bbox.Height()-1, std::ceil(m_pos.x + m_bbox.Width()), m_pos.y + m_bbox.Height() };
}
Trect<double> MovingEntity::GetLRect(void)
{
	return { m_pos.x - 1, m_pos.y, m_pos.x , m_pos.y + m_bbox.Height() };
}
Trect<double> MovingEntity::GetRRect(void)
{
	return { m_pos.x + m_bbox.Width(), m_pos.y, m_pos.x + m_bbox.Width() + 1, m_pos.y + m_bbox.Height() };
}
