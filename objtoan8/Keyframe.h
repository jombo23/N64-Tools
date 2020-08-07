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
	bool rotationHasDerivative[3];
	float3 rotationDerivativeValue;
	float3 translation;
	bool translationIsInterpolated[3];
	bool translationHasDerivative[3];
	float3 translationDerivativeValue;
	float3 scale;
	bool scaleIsInterpolated[3];
	bool scaleHasDerivative[3];
	float3 scaleDerivativeValue;

	CNullableFloat fieldOfView;
	CColorQuadruplet envColor;
};
