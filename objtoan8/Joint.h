#pragma once

#include "float3.h"
#include <vector>

using namespace math;


class CJoint
{
public:
	CJoint(void);
	~CJoint(void);

	CString name;
	CJoint* parent;
	std::vector<CJoint*> children;

	float3 positionAbsolute;

	float3 orientationVectorX;
	float3 orientationVectorY;
	float3 orientationVectorZ;

	std::vector<int> controlPoints;
};
