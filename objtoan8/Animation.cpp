#include "StdAfx.h"
#include "Animation.h"

CAnimation::CAnimation(void)
{
	isCamera = false;
	treatAsBlendShape = false;
	rotationOrder = "XYZ";
}

CAnimation::~CAnimation(void)
{
}
