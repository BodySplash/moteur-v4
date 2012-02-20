#include "cgolist.h"

CGoList::CGoList(void)
{
	_previous = NULL;
	_current = NULL;
	_next = NULL;
}

CGoList::~CGoList(void)
{
	if (_current)
		delete _current;
}

