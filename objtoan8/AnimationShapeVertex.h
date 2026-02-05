#pragma once
#include <vector>
#include "float3.h"

class CAnimationShapeVertex
{
public:
	CAnimationShapeVertex();
	~CAnimationShapeVertex();

	int vertexIndex;
	float3 translation;
};

