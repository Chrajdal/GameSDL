#include "MovingEntity.h"

MovingEntity::MovingEntity()
{
}

MovingEntity::~MovingEntity()
{
}

bool MovingEntity::GetTColl(const QuadTree & terrain)
{
	std::vector<const Node *> ceiling = terrain.range(GetTRect());
	for (const auto & i : ceiling)
		if (i->m_tile != tile_type::air)
			return true;
	return false;
}

bool MovingEntity::GetBColl(const QuadTree & terrain)
{
	std::vector<const Node *> floor = terrain.range(GetBRect());
	for (const auto & i : floor)
		if (i->m_tile != tile_type::air)
			return true;
	return false;
}

bool MovingEntity::GetLColl(const QuadTree & terrain)
{
	std::vector<const Node *> left_wall = terrain.range(GetLRect());
	for (const auto & i : left_wall)
		if (i->m_tile != tile_type::air)
			return true;
	return false;
}

bool MovingEntity::GetRColl(const QuadTree & terrain)
{
	std::vector<const Node *> right_wall = terrain.range(GetRRect());	
	for (const auto & i : right_wall)
		if (i->m_tile != tile_type::air)
			return true;
	return false;
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
	return { m_pos.x - 1,std::floor(m_pos.y), m_pos.x , std::ceil(m_pos.y + m_bbox.Height()-0.1) };
}
Trect<double> MovingEntity::GetRRect(void)
{
	return { m_pos.x + m_bbox.Width()-1, std::floor(m_pos.y), m_pos.x + m_bbox.Width() , std::ceil(m_pos.y + m_bbox.Height()-0.1) };
}
