// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8FloatKey::An8FloatKey(void)
{
	NbrFloat = 0;
	FrameNumber = 0;
}

An8FloatKey::~An8FloatKey(void)
{

}


bool An8FloatKey::Load(AN8XPARSER*p_parser)
{
	p_parser->GetName(&KnotBehavior);

	string line = p_parser->SubStr("{ ","\"");
	p_parser->SetLine(line);
	string element = p_parser->SubStr(""," ");
	FrameNumber = atoi( element.c_str());

	p_parser->CutLine(" ",1);

	NbrFloat = p_parser->CountChar(" ");

	float newFloat = 0.0f;
	for( int i = 0 ; i < NbrFloat; ++i )
	{
		string element = p_parser->SubStr(""," ");
		newFloat = (float)atof( element.c_str());
		vFloats.push_back(newFloat);
		p_parser->CutLine(" ",1);
	}


	return true;
}

