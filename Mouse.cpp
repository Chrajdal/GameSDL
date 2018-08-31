#include "Mouse.h"



Mouse::Mouse()
	: x(0), y(0)
{
	std::fill(MouseStates.begin(), MouseStates.end(), false);
}


Mouse::~Mouse()
{
}
