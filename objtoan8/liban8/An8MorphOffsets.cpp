// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8MorphOffsets::An8MorphOffsets(void)
{

}

An8MorphOffsets::~An8MorphOffsets(void)
{

}


void An8MorphOffsets::Load(AN8XPARSER*p_parser)
{
	if(p_parser->FindWord("name {"))
		p_parser->GetName(&Name);

	p_parser->ReadLine();

	std::string strMorph = "";
	while(p_parser->FindWord("(") || p_parser->FindWord(")"))
	{
		strMorph += p_parser->GetLine();
		p_parser->ReadLine();
	}

	p_parser->SetLine(strMorph);
	while( p_parser->FindWord("(") )
	{
		++NbrMorphOffset;
		An8MorphOffset newMorphOffset;

		string element;
		p_parser->CaptureNumber(&element);
		newMorphOffset.Index = atoi( element.c_str() );

		p_parser->CutLine("(",1);
		element = p_parser->SubStr(""," ");
		newMorphOffset.Position.x = (float)atof( element.c_str() );

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		newMorphOffset.Position.y = (float)atof( element.c_str() );

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr("",")");
		newMorphOffset.Position.z = (float)atof( element.c_str() );

		vMorphOffsets.push_back(newMorphOffset);

		p_parser->CutLine(")",1);
	}

}
