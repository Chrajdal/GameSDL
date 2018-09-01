#pragma once

#include "Geometry.h"
#include "Utils.h"

class Moveable
{
public:
	Moveable();
	~Moveable();

// todo: protected:
public:
	v2d m_pos;
	v2d m_vel;
};

