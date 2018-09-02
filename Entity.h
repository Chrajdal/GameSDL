#pragma once

#include <memory>
#include <SDL.h>

#include "Geometry.h"
#include "Utils.h"

class Entity
{
public:
	Entity();
	~Entity();

// todo: protected:
public:
	v2d m_pos;
	v2d m_vel;

};

