#pragma once

#include "float3.h"
#include "AnimationShapeVertex.h"

using namespace math;

class CAnimationPart
{
public:
	CAnimationPart(void);
	~CAnimationPart(void);

	CString name;
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

	std::vector<CAnimationShapeVertex> shapeVertices;
};
