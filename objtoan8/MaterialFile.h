#pragma once

#include <vector>
#include "Material.h"

class CMaterialFile
{
public:
	CMaterialFile(void);
	~CMaterialFile(void);

	std::vector<CMaterial*> materials;
	CString name;

};
