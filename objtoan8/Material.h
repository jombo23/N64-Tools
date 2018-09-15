#pragma once

#include "ColorTriplet.h"
#include "NullableFloat.h"
#include "NullableString.h"

class CMaterial
{
public:
	CMaterial(void);
	~CMaterial(void);

	CString name;

	CColorTriplet ambientColor;
	CColorTriplet diffuseColor;
	CColorTriplet spectralColor;
	CNullableFloat alpha;
	CNullableFloat phongSpectral;
	CNullableFloat illum;
	CNullableString diffuseTexture;
	
};
