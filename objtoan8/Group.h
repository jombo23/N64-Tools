#pragma once

#include <vector>
#include "PolygonSet.h"


class CGroup
{
public:
	CGroup(void);
	~CGroup(void);

	std::vector<CPolygon*> polygons;
	CString name;
};
