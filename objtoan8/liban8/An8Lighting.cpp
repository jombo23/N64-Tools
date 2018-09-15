// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// Web Site: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Lighting::An8Lighting(void)
{
}

An8Lighting::~An8Lighting(void)
{

}

bool An8Lighting::Load(AN8XPARSER*p_parser)
{
	int elementStart = p_parser->IndexOf(1,"lighting");
	if(elementStart<0) return false;

	int elementEnd = p_parser->IndexOf(1,"}");
	while(p_parser->Length()!=1+LINERETURN && elementStart != elementEnd)
	{
		if(p_parser->FindWord("intensity {"))
		{
			p_parser->CutLine("{",2);
			string element = p_parser->SubStr(""," ");
			Intensity = (float)atof( element.c_str());
		}
		if(p_parser->FindWord("ambientintensity {"))
		{
			p_parser->CutLine("{",2);
			string element = p_parser->SubStr(""," ");
			AmbientIntensity = (float)atof( element.c_str());
		}
		p_parser->ReadLine();
		elementEnd =p_parser->IndexOf(1,"}");
	}
	return true;
}
