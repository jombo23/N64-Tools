// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// Web Site: texel3d.free.fr
#include "An8Loader.h"
using namespace std;
An8Dof::An8Dof(void)
{
	Min = 0.0f;
	Default = 0.0f;
	Max = 0.0f;
	Locked = false;
	Unlimited = false;
}

An8Dof::~An8Dof(void)
{

}

void An8Dof::Load(AN8XPARSER*p_parser)
{
	string element;
	if(p_parser->FindWord("dof {"))
	{
		p_parser->GetName(&Axe);

		p_parser->CutLine("\"",1);
		p_parser->CutLine("\"",1);
		int lastNumberPos = p_parser->CaptureNumber(&element);
		Min = (float) atof( element.c_str());

		p_parser->SubLine(lastNumberPos,p_parser->Length() - lastNumberPos);
		p_parser->CutLine(" ",0);
		lastNumberPos = p_parser->CaptureNumber(&element);
		Default = (float) atof( element.c_str());

		p_parser->SubLine(lastNumberPos,p_parser->Length() - lastNumberPos);
		p_parser->CutLine(" ",0);
		lastNumberPos = p_parser->CaptureNumber(&element);
		Max = (float) atof( element.c_str());
	}
	while( !p_parser->FindWord("}"))
	{
		p_parser->ReadLine();
		if( p_parser->FindWord("unlimited {"))
		{
			Unlimited = true;
			p_parser->SetLine("VOID");
		}
		else if( p_parser->FindWord("locked {"))
		{
			Locked = true;
			p_parser->SetLine("VOID");
		}
	}

}
