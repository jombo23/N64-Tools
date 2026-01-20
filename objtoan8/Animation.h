#pragma once

#include <vector>
#include "Keyframe.h"

class CAnimation
{
public:
	CAnimation(void);
	~CAnimation(void);

	CString name;
	std::vector<CKeyframe*> keyframes;

	bool isCamera;
	bool treatAsBlendShape;
	CString rotationOrder;
};
