#pragma once

#include "Utils.h"
#include "Graphics.h"
#include "Color.h"
#include <vector>
#include "Tile.h"


class Node
{
public:
	Node(int x, int y, const tile_type & ttile, const Trect<double> & boundary = {});
	Node(const Node & src);
	Node & operator = (const Node & src);
	~Node(void);

	bool insert(const Node & n);
	void range(std::vector<const Node *> & PointsInRange, const Trect<double> & range) const;
	const Node * at(int x, int y) const;
	Node * access(int x, int y);

	friend std::ostream & operator << (std::ostream & os, const Node & src)
	{
		if (src.m_nw != nullptr) os << *src.m_nw;
		if (src.m_ne != nullptr) os << *src.m_ne;
		if (src.m_sw != nullptr) os << *src.m_sw;
		if (src.m_se != nullptr) os << *src.m_se;

		return os << src.m_x << " " << src.m_y << " " << (int)src.m_tile << " # ";
	}

	int m_x;
	int m_y;
	Node * m_nw, *m_ne, *m_sw, *m_se;
	Trect<double> m_boundary;
	tile_type m_tile;
};