// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr

#include "An8Loader.h"
using namespace std;

An8Attribute::An8Attribute(void)
{
	Type = AN8_UNDEFINED; // float, string, position, ... ?

	FloatValue   = 0.0f;
	IntegerValue = 0;
	StringValue  = "";
}

An8Attribute::~An8Attribute(void)
{

}

void An8Attribute::Load(AN8XPARSER*p_parser)
{
	string element;

	p_parser->GetName(&Name);

	p_parser->CutLine("\"",1);
	p_parser->CutLine("\"",1);

	element = p_parser->SubStr(" "," ");

	if( element.compare("int") == 0 )
	{
		Type = AN8_INTEGER;
		element = p_parser->SubStr(" int "," }");
		IntegerValue = atoi( element.c_str());
	}
	else if( element.compare("float") == 0 )
	{
		Type = AN8_FLOAT;
		element = p_parser->SubStr(" float "," }");
		FloatValue = (float)atof( element.c_str());
	}
	else if( element.compare("point3") == 0 )
	{
		Type = AN8_POINT3;

		p_parser->CutLine("(",1);
		element = p_parser->SubStr(""," ");
		Point3Value.x = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		Point3Value.y = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr("",")");
		Point3Value.z = (float)atof( element.c_str());
	}
	else if( element.compare("quaternion") == 0 )
	{
		Type = AN8_QUATERNION;

		p_parser->CutLine("(",1);
		element = p_parser->SubStr(""," ");
		QuaternionValue.x = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		QuaternionValue.y = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		QuaternionValue.z = (float)atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr("",")");
		QuaternionValue.w = (float)atof( element.c_str());
	}
	else if( element.compare("string") == 0 )
	{
		Type = AN8_STRING;

		// First line
		if(p_parser->FindWord("\"")) // string in one line
		{
			p_parser->CutLine("\"",1);
			element = p_parser->GetLine();
		}
		else // string start in the next line
		{
			p_parser->ReadLine();
			p_parser->CutLine("\"",1);
			element = p_parser->GetLine();
		}

		int pos = (int)element.rfind("\"",element.length()-1);

		StringValue = element.substr(0,pos);
	}
	p_parser->SetLine("");
}
