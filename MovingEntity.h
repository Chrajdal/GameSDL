#pragma once

#include <algorithm>

#include "Entity.h"

class MovingEntity : public Entity
{
public:
	MovingEntity();
	~MovingEntity();

	virtual bool GetTColl(const QuadTree & terrain);
	virtual bool GetBColl(const QuadTree & terrain);
	virtual bool GetLColl(const QuadTree & terrain);
	virtual bool GetRColl(const QuadTree & terrain);

	virtual Trect<double> GetTRect(void);
	virtual Trect<double> GetBRect(void);
	virtual Trect<double> GetLRect(void);
	virtual Trect<double> GetRRect(void);
	
public:
	v2d m_vel;
	v2d m_acc;
	Trect<double> m_bbox;
};

