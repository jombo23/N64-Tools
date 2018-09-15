// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8TexGen::An8TexGen(void)
{
}

An8TexGen::~An8TexGen(void)
{

}


void An8TexGen::Load(AN8XPARSER*p_parser)
{
	////Name/////////////
	p_parser->ReadLine();
	p_parser->GetName(&Mode);

	// Scale
	p_parser->ReadLine();
	if(p_parser->FindWord("scale {"))
	{
		p_parser->CutLine("{",2);
		string element = p_parser->SubStr(""," ");
		Scale.x = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		Scale.y = (float)atof( element.c_str());
	}
	else
	{
		throw "scale { not found in texgen.";
	}


    // Center
	p_parser->ReadLine();
	if(p_parser->FindWord("center {"))
	{
		p_parser->CutLine("(",1);
		string element = p_parser->SubStr(""," ");
		Center.x = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		Center.y = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr("",")");
		Center.z = (float)atof( element.c_str());

	}
	else
	{
		throw "center { not found in texgen.";
	}


	// Up
	p_parser->ReadLine();
	if(p_parser->FindWord("up {"))
	{
		p_parser->CutLine("(",1);
		string element = p_parser->SubStr(""," ");
		Up.x = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		Up.y = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr("",")");
		Up.z = (float)atof( element.c_str());
	}
	else
	{
		throw "up { not found in texgen.";
	}

	// Forward
	p_parser->ReadLine();
	if(p_parser->FindWord("forward {"))
	{
		p_parser->CutLine("(",1);
		string element = p_parser->SubStr(""," ");
		Forward.x = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		Forward.y = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr("",")");
		Forward.z = (float)atof( element.c_str());
	}
	else
	{
		throw "forward { not found in texgen.";
	}

}
