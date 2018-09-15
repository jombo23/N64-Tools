#include "StdAfx.h"
#include "Bone.h"

CBone::CBone(void)
{
	boneStart = NULL;
	boneEnd = NULL;
	parentBone = NULL;
	length = 0.0;
}

CBone::~CBone(void)
{
}