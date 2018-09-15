#pragma once

#include "NullableClass.h"

class CColorTriplet : public CNullableClass
{
public:
	CColorTriplet(void);
	~CColorTriplet(void);

	float r;
	float g;
	float b;
};
