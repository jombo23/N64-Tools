#pragma once
#include "nullableclass.h"

class CNullableFloat :
	public CNullableClass
{
public:
	CNullableFloat(void);
	~CNullableFloat(void);

	float floatValue;
};
