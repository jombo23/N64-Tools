// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Controller::An8Controller(void)
{
	HasTrack = false;
	IsUser   = false;
}

An8Controller::~An8Controller(void)
{

}


bool An8Controller::Load(AN8XPARSER*p_parser)
{
	string element;
	p_parser->GetName(&Name);


	int controllerStart =p_parser->IndexOf(1,"controller {");
	if(controllerStart<0) return false;
	int controllerEnd =p_parser->IndexOf(1,"}");

	while(p_parser->Length()!=1+LINERETURN && controllerStart != controllerEnd) 
	{
		if(p_parser->FindWord("script {"))
		{
			int scriptStart = p_parser->IndexOf(1,"script {");
			int scriptEnd   = p_parser->IndexOf(1,"}");
			while(p_parser->Length()!=1+LINERETURN && scriptStart != scriptEnd) 
			{
				p_parser->ReadLine(); // skip
				scriptEnd   = p_parser->IndexOf(1,"}");
			}

			
		}
		if(p_parser->FindWord("track {"))
		{
			HasTrack = true;
			Track.Load(p_parser);
		}
		if(p_parser->FindWord("user {"))
		{
			IsUser = true;
		}
		if(p_parser->FindWord("type {"))
		{
			p_parser->GetName(&Type);
		}
		p_parser->ReadLine();
	    controllerEnd =p_parser->IndexOf(1,"}");
	}
	
	return true;
}

