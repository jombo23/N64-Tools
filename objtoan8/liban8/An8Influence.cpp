// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Influence::An8Influence(void)
{
	CenterStart    = 0.0f;
	InRadiusStart  = 0.0f;
	OutRadiusStart = 0.0f;

	CenterEnd    = 0.0f;
	InRadiusEnd  = 0.0f;
	OutRadiusEnd = 0.0f;
}

An8Influence::~An8Influence(void)
{

}

void An8Influence::Load(AN8XPARSER*p_parser)
{
	/*
	if(p_parser->FindWord("influence {"))
	{
		p_parser->CutLine("{",2);
		string element = p_parser->SubStr(""," ");
		CenterStart = (float) atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		InRadiusStart = (float) atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		OutRadiusStart = (float) atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		CenterEnd = (float) atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		InRadiusEnd = (float) atof( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		OutRadiusEnd = (float) atof( element.c_str());
	}
	*/
	if(p_parser->FindWord("influence {"))
	{
		std::string str = p_parser->GetLine(); // the line
		string element; // will store numbers
		
		// First element to get
		int pos = p_parser->StrCaptureNumber(&element,&str);
		if( pos == -1 )
		{
			p_parser->ReadLine();
			str = p_parser->GetLine();
			pos = p_parser->StrCaptureNumber(&element,&str);
		}
		if( pos > -1 )
		{
			CenterStart = (float) atof( element.c_str());
		}

		// Second
		str = str.substr(pos+element.length(), str.length() - pos - element.length() );
		pos = p_parser->StrCaptureNumber(&element,&str);
		if( pos == -1 )
		{
			p_parser->ReadLine();
			str = p_parser->GetLine();
			pos = p_parser->StrCaptureNumber(&element,&str);
		}
		if( pos > -1 )
		{
			InRadiusStart = (float) atof( element.c_str());
		}

		// 
		str = str.substr(pos+element.length(), str.length() - pos - element.length() );
		pos = p_parser->StrCaptureNumber(&element,&str);
		if( pos == -1 )
		{
			p_parser->ReadLine();
			str = p_parser->GetLine();
			pos = p_parser->StrCaptureNumber(&element,&str);
		}
		if( pos > -1 )
		{
			OutRadiusStart = (float) atof( element.c_str());
		}

		// 
		str = str.substr(pos+element.length(), str.length() - pos - element.length() );
		pos = p_parser->StrCaptureNumber(&element,&str);
		if( pos == -1 )
		{
			p_parser->ReadLine();
			str = p_parser->GetLine();
			pos = p_parser->StrCaptureNumber(&element,&str);
		}
		if( pos > -1 )
		{
			CenterEnd = (float) atof( element.c_str());
		}

		// 
		str = str.substr(pos+element.length(), str.length() - pos - element.length() );
		pos = p_parser->StrCaptureNumber(&element,&str);
		if( pos == -1 )
		{
			p_parser->ReadLine();
			str = p_parser->GetLine();
			pos = p_parser->StrCaptureNumber(&element,&str);
		}
		if( pos > -1 )
		{
			InRadiusEnd = (float) atof( element.c_str());
		}
		
		// 
		str = str.substr(pos+element.length(), str.length() - pos - element.length() );
		pos = p_parser->StrCaptureNumber(&element,&str);
		if( pos == -1 )
		{
			p_parser->ReadLine();
			str = p_parser->GetLine();
			pos = p_parser->StrCaptureNumber(&element,&str);
		}
		if( pos > -1 )
		{
			OutRadiusEnd = (float) atof( element.c_str());
		}

	}
}
