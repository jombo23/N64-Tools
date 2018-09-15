// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8PointKey::An8PointKey(void)
{
	FrameNumber = 0;
	KnotBehavior = "";
}

An8PointKey::~An8PointKey(void)
{

}


bool An8PointKey::Load(AN8XPARSER*p_parser)
{
	p_parser->GetName(&KnotBehavior);

	string line = p_parser->SubStr("{ ","\"");
	p_parser->SetLine(line);
	string element = p_parser->SubStr(""," ");
	FrameNumber = atoi( element.c_str());

	// Value
	p_parser->CutLine("(",1);
	element = p_parser->SubStr(""," ");
	Value.x = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr(""," ");
	Value.y = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr("",")");
	Value.z = (float)atof( element.c_str());


	// Forward
	p_parser->CutLine("(",1);
	element = p_parser->SubStr(""," ");
	Forward.x = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr(""," ");
	Forward.y = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr("",")");
	Forward.z = (float)atof( element.c_str());

	// Reverse
	p_parser->CutLine("(",1);
	element = p_parser->SubStr(""," ");
	Reverse.x = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr(""," ");
	Reverse.y = (float)atof( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr("",")");
	Reverse.z = (float)atof( element.c_str());

	return true;
}

