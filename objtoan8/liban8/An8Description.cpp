// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// Web Site: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Description::An8Description(void)
{
}

An8Description::~An8Description(void)
{
}

bool An8Description::Load(AN8XPARSER*p_parser)
{
	int elementStart = p_parser->IndexOf(1,"description");
	if(elementStart<0) return false;

	int elementEnd = p_parser->IndexOf(1,"}");
	while( /*p_parser->Length()!=1+LINERETURN*/ p_parser->IndexOf(1, "}")!= 0 && elementStart != elementEnd)
	{
		if(p_parser->FindWord("\""))
		{
			std::string str = p_parser->GetLine();
			int start = (int)str.find("\"",0);
			String += str.substr(start+1,str.length()-start-3 );
		}
		p_parser->ReadLine();
		elementEnd =p_parser->IndexOf(1,"}");
	}
	return true;
}
