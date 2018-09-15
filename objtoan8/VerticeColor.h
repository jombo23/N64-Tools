#pragma once
#include "ColorQuadruplet.h"

class CVerticeColor : public CNullableClass
{
public:
	CVerticeColor(void);
	~CVerticeColor(void);

	CColorQuadruplet color;
};
