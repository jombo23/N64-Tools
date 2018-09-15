// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8QKey::An8QKey(void)
{
	FrameNumber = 0;
	KnotBehavior = "";
}

An8QKey::~An8QKey(void)
{

}


bool An8QKey::Load(AN8XPARSER*p_parser)
{
	p_parser->GetName(&KnotBehavior);

	string line = p_parser->SubStr("{ ","\"");
	p_parser->SetLine(line);
	string element = p_parser->SubStr(""," ");
	FrameNumber = atoi( element.c_str());

	p_parser->CutLine("(",1);
	element = p_parser->SubStr(""," ");
	Quaternion.x = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr(""," ");
	Quaternion.y = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr(""," ");
	Quaternion.z = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr("",")");
	Quaternion.w = (float)atof( element.c_str());


	return true;
}

