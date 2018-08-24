#include "QuadTree.h"

QuadTree::QuadTree(void)
	: m_root(NULL), m_size(0)
{}

QuadTree::~QuadTree(void)
{
	delete m_root;
}

void QuadTree::insert(const Node & n)
{
	if (m_root == NULL)
	{
		Trect<double> boundary{ { -100000, -100000 },{ 100000, 100000 } };
		if (boundary.contains(n.m_x, n.m_y))
		{
			m_root = new Node(n.m_x, n.m_y, n.m_tile, boundary);
			m_size = 1;
		}
	}
	else
	{
		if (m_root->insert(n))
			m_size++;
	}
}

std::vector<const Node *> QuadTree::range(const Trect<double> & range) const
{
	std::vector<const Node *> res;
	if (m_root != NULL)
		m_root->range(res, range);
	return res;
}

const Node * QuadTree::at(int x, int y) const
{
	if (m_root == NULL)
		return NULL;
	else
	{
		const Node * n = NULL;
		return m_root->at(x, y, n);
	}
}

Node * QuadTree::access(int x, int y)
{
	if (m_root == NULL)
		return NULL;
	else
		return m_root->access(x, y);
}

void QuadTree::clear(void)
{
	delete m_root;
	m_root = NULL;
	m_size = 0;
}

unsigned QuadTree::size(void) const
{
	return m_size;
}

Trect<double> QuadTree::boundary(void) const
{
	if (m_root == NULL)
		return Trect<double>();
	return m_root->m_boundary;
}

void QuadTree::SaveToFile(const std::string & file_name)
{
	std::ofstream file(file_name, std::fstream::out);
	if (!file.is_open())
		throw file_name + " cannot be opened.";

	if (m_root != NULL)
	{
		file << *m_root;
	}
	file.close();
}

void QuadTree::LoadFromFile(const std::string & file_name)
{
	std::ifstream tmp_file(file_name);
	if (!tmp_file.is_open())
	{
		this->SaveToFile(file_name);
	}

	std::ifstream file(file_name);

	if (m_root != NULL)
	{
		delete m_root;
		m_root = NULL;
		m_size = 0;
	}

	while (true)
	{
		int x;
		int y;
		int tmp_tile;
		char dummy;
		file >> x >> y >> tmp_tile >> dummy;

		this->insert(Node(x, y, static_cast<tile_type>(tmp_tile)));

		if (file.eof())
			break;
	}
	file.close();
}