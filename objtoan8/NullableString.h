#pragma once
#include "nullableclass.h"

class CNullableString :
	public CNullableClass
{
public:
	CNullableString(void);
	~CNullableString(void);

	CString stringValue;
};
