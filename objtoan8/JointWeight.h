#pragma once
#include "Vertice.h"
#include "Joint.h"

class CJointWeight
{
public:
	CJointWeight(void);
	~CJointWeight(void);

	CJoint* joint;
	int subIndexNumber;
	float weight;
};
