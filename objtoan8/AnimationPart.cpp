#include "StdAfx.h"
#include "AnimationPart.h"

CAnimationPart::CAnimationPart(void)
{
	rotation = float3(0,0,0);
	scale = float3(1,1,1);
	translation = float3(0,0,0);
}

CAnimationPart::~CAnimationPart(void)
{
}
