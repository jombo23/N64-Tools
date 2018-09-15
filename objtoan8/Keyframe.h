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
	float3 translation;
	float3 scale;

	CNullableFloat fieldOfView;
	CColorQuadruplet envColor;
};
