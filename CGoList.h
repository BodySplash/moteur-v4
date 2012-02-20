#pragma once


#include "CGo.h"

class CGoList
{
public:
	CGoList* _previous;
	CGo* _current;
	CGoList* _next;

	CGoList(void);
	~CGoList(void);
	
};
