// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Figure::An8Figure(void)
{
	NbrMaterial = 0;
}

An8Figure::~An8Figure(void)
{

}

bool An8Figure::Load(AN8XPARSER*p_parser)
{
	// Name
	p_parser->GetName(&Name);

	while( p_parser->IndexOf(1, "}")!= 0   ) // While we do not find the end of the object
	{

		p_parser->ReadLine();
		
		if(p_parser->FindWord("material {"))
		{
			++NbrMaterial;
			An8Material newMaterial;
			vMaterials.push_back(newMaterial);
			vMaterials[vMaterials.size()-1].Load(p_parser);
			
		} // End of if material
		

		if(p_parser->FindWord("bone {"))
		{
			Root.Load(p_parser,vMaterials);
		}

	}
	return true;
}
