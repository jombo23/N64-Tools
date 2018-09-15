#pragma once

#include "float3.h"

using namespace math;

class CAnimationPart
{
public:
	CAnimationPart(void);
	~CAnimationPart(void);

	CString name;
	float3 rotation;
	float3 scale;
	float3 translation;
};
