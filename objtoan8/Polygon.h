#pragma once

#include <vector>
#include "PolygonPoint.h"

class CPolygon
{
public:
	CPolygon(void);
	~CPolygon(void);

	std::vector<CPolygonPoint*> polygonPoints;
	CString materialName;
};
