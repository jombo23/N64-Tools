#include "StdAfx.h"
#include "Keyframe.h"

CKeyframe::CKeyframe(void)
{
	translation = float3(0,0,0);
	rotation = float3(0,0,0);
	scale = float3(1,1,1);
	number = -1;
}

CKeyframe::~CKeyframe(void)
{
}
