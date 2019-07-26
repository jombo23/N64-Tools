#include "StdAfx.h"
#include "Joint.h"

CJoint::CJoint(void)
{
	parent = NULL;
	positionAbsolute = float3(0,0,0);
	orientationVectorX = float3(1,0,0);
	orientationVectorY = float3(0,1,0);
	orientationVectorZ = float3(0,0,1);
	jointType = Absolute;
}

CJoint::~CJoint(void)
{
}
