#pragma once

#include "float3.h"
#include <vector>

using namespace math;

enum JointType { Absolute, Relative };

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

	float3 scaleRelative;
	float3 rotationRelative;
	float3 positionRelative;

	std::vector<int> controlPoints;
	JointType jointType;
};
