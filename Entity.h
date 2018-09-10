#pragma once

#include <memory>
#include <SDL.h>
#include <list>

#include "Geometry.h"
#include "Utils.h"
#include "QuadTree.h"

class Entity
{
public:
	Entity();
	~Entity();
	
// todo: protected:
public:
	v2d m_pos;
};

