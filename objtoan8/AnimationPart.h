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
	bool rotationIsInterpolated[3];
	float3 translation;
	bool translationIsInterpolated[3];
	float3 scale;
	bool scaleIsInterpolated[3];
};
