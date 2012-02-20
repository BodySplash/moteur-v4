#include "cgo.h"

CGo::CGo(void)
{
	_pos.x = 0;
	_pos.y = 0;
}

void CGo::setPos(vertexCoord newPos){
	_pos.x = newPos.x;
	_pos.y = newPos.y;
}

CGo::~CGo(void)
{
}
