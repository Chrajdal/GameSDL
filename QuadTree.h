#pragma once

#include "Utils.h"
#include "Graphics.h"
#include "Color.h"
#include "node.h"
#include <vector>
#include <fstream>
#include <sstream>

class QuadTree
{
public:
	QuadTree(void);
	~QuadTree(void);

	void insert(const Node & n);
	std::vector<const Node *> range(const Trect<double> & range) const;
	const Node * at(int x, int y) const;

	Node * access(int x, int y);

	void clear(void);
	unsigned size(void) const;
	Trect<double> boundary(void) const;

	void SaveToFile(const std::string & file_name);
	void LoadFromFile(const std::string & file_name);


private:
	Node * m_root;
	unsigned m_size;
};