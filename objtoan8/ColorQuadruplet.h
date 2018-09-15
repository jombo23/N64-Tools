#pragma once
#include "nullableclass.h"

class CColorQuadruplet :
	public CNullableClass
{
public:
	CColorQuadruplet(void);
	~CColorQuadruplet(void);

	float r;
	float g;
	float b;
	float a;
};
