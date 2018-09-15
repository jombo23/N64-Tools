#pragma once

#include "Joint.h"
#include "float3.h"
#include "vector"
#include "Quat.h"
#include "Liban8/An8Loader.h"

class CBone
{
public:
	CBone(void);
	~CBone(void);

	CJoint* boneStart;
	CJoint* boneEnd;
	CString name;

	CBone* parentBone;
	std::vector<CBone*> boneConnections;

	AN8XQUATERNION orientation;
	Quat overallOrientation;
	float length;
};
