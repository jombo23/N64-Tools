#pragma once

#include <vector>
#include "AnimationPart.h"
#include "float3.h"
#include "ColorQuadruplet.h"
#include "NullableFloat.h"

using namespace math;

class CKeyframe
{
public:
	CKeyframe(void);
	~CKeyframe(void);

	int number;
	std::vector<CAnimationPart*> animationParts;

	float3 rotation;
	bool rotationIsInterpolated[3];
	float3 translation;
	bool translationIsInterpolated[3];
	float3 scale;
	bool scaleIsInterpolated[3];

	CNullableFloat fieldOfView;
	CColorQuadruplet envColor;
};
