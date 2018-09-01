#include "node.h"

Node::Node(int x, int y, const tile_type & ttile, const Trect<double> & boundary)
	: m_x(x), m_y(y),
	m_nw(NULL), m_ne(NULL), m_sw(NULL), m_se(NULL),
	m_tile(ttile), m_boundary(boundary)
{}
Node::Node(const Node & src)
	: m_x(src.m_x), m_y(src.m_y),
	m_nw(NULL), m_ne(NULL), m_sw(NULL), m_se(NULL),
	m_tile(src.m_tile),
	m_boundary(src.m_boundary)
{}

Node & Node::operator = (const Node & src)
{
	if (this == &src)
		return *this;

	delete m_nw; delete m_ne; delete m_sw; delete m_se;

	m_nw = NULL;
	m_ne = NULL;
	m_sw = NULL;
	m_se = NULL;
	m_tile = src.m_tile;
	m_x = src.m_x;
	m_y = src.m_y;
	m_boundary = src.m_boundary;
	return *this;
}

Node::~Node(void)
{
	delete m_nw; delete m_ne; delete m_sw; delete m_se;
}

bool Node::insert(const Node & n)
{
	if (m_x == n.m_x && m_y == n.m_y)
		return false;

	if (!m_boundary.contains(n.m_x, n.m_y))
		return false;

	Tpoint<double> middle({ (m_boundary.m_upleft.m_x + m_boundary.m_downright.m_x) / 2,
		(m_boundary.m_upleft.m_y + m_boundary.m_downright.m_y) / 2 });
	if (n.m_x < middle.m_x) // WEST
	{
		if (n.m_y < middle.m_y) // NORTH
		{
			if (m_nw == NULL)
			{
				m_nw = new Node(n.m_x, n.m_y, n.m_tile, { m_boundary.m_upleft, middle });
				return true;
			}
			else
				return m_nw->insert(n);
		}
		else // SOUTH
		{
			if (m_sw == NULL)
			{
				m_sw = new Node(n.m_x, n.m_y, n.m_tile,
					{ { m_boundary.m_upleft.m_x, middle.m_y },
					{ middle.m_x, m_boundary.m_downright.m_y } });
				return true;
			}
			else
				return m_sw->insert(n);
		}
	}
	else //  EAST
	{
		if (n.m_y < middle.m_y) // NORTH
		{
			if (m_ne == NULL)
			{
				m_ne = new Node(n.m_x, n.m_y, n.m_tile,
					{ { middle.m_x, m_boundary.m_upleft.m_y },
					{ m_boundary.m_downright.m_x, middle.m_y } });
				return true;
			}
			else
				return m_ne->insert(n);

		}
		else // SOUTH
		{
			if (m_se == NULL)
			{
				m_se = new Node(n.m_x, n.m_y, n.m_tile, { middle, m_boundary.m_downright });
				return true;
			}
			else
				return m_se->insert(n);
		}
	}
}

void Node::range(std::vector<const Node *> & PointsInRange, const Trect<double> & range) const
{
	if (range.contains(m_x, m_y))
		PointsInRange.push_back(this);

	if (m_nw != NULL)
	{
		if (range.CheckCollide(m_nw->m_boundary))
			m_nw->range(PointsInRange, range);
	}
	if (m_ne != NULL)
	{
		if (range.CheckCollide(m_ne->m_boundary))
			m_ne->range(PointsInRange, range);
	}
	if (m_sw != NULL)
	{
		if (range.CheckCollide(m_sw->m_boundary))
			m_sw->range(PointsInRange, range);
	}
	if (m_se != NULL)
	{
		if (range.CheckCollide(m_se->m_boundary))
			m_se->range(PointsInRange, range);
	}
}

const Node * Node::at(int x, int y) const
{
	if (m_x == x && m_y == y)
		return this;
	if (m_boundary.contains(x, y))
	{
		if (m_nw != NULL)
		{
			if (m_nw->m_boundary.contains(x, y))
				return m_nw->access(x, y);
		}
		if (m_ne != NULL)
		{
			if (m_ne->m_boundary.contains(x, y))
				return m_ne->access(x, y);
		}
		if (m_sw != NULL)
		{
			if (m_sw->m_boundary.contains(x, y))
				return m_sw->access(x, y);
		}
		if (m_se != NULL)
		{
			if (m_se->m_boundary.contains(x, y))
				return m_se->access(x, y);
		}
	}
	return NULL;
}

Node * Node::access(int x, int y)
{
	if (m_x == x && m_y == y)
		return this;
	if (m_boundary.contains(x, y))
	{
		if (m_nw != NULL)
		{
			if (m_nw->m_boundary.contains(x, y))
				return m_nw->access(x, y);
		}
		if (m_ne != NULL)
		{
			if (m_ne->m_boundary.contains(x, y))
				return m_ne->access(x, y);
		}
		if (m_sw != NULL)
		{
			if (m_sw->m_boundary.contains(x, y))
				return m_sw->access(x, y);
		}
		if (m_se != NULL)
		{
			if (m_se->m_boundary.contains(x, y))
				return m_se->access(x, y);
		}
	}
	return NULL;
}
