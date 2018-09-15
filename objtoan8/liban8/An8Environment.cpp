// This file in a part of LibAn8, Copyright (c) 2002-2008, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Environment::An8Environment(void)
{
	IsAutoGrid = 1;
	ModelingGridSpacing = 10.0f;
	SceneEditorGridSpacing = 50.0f;
	GroundFloorGridSize = 50.0f;
	FrameRate = 24;
}

An8Environment::~An8Environment(void)
{

}

/////////////////////////////////////
// Load the environment
/////////////////////////////////////
bool An8Environment::Load(AN8XPARSER*p_parser)
{

   p_parser->ReadLine();

	while(p_parser->Length()!=1+LINERETURN) //while(!p_parser->FindWord("environment"))
	{
		if(p_parser->FindWord("grid {"))
		{
			p_parser->CutLine("{",2);
			string element = p_parser->SubStr(""," ");
			IsAutoGrid = atoi( element.c_str());

			p_parser->CutLine(" ",1);
			element = p_parser->SubStr(""," ");
			ModelingGridSpacing = (float)atof( element.c_str());

			p_parser->CutLine(" ",1);
			element = p_parser->SubStr(""," ");
			SceneEditorGridSpacing = (float)atof( element.c_str());

			p_parser->CutLine(" ",1);
			element = p_parser->SubStr(""," ");
			GroundFloorGridSize = (float)atof( element.c_str());
		}
		if(p_parser->FindWord("framerate {"))
		{
			p_parser->CutLine("{",1);
			string element = p_parser->SubStr(" "," ");
			FrameRate = atoi( element.c_str());
		}
		if(p_parser->FindWord("film {"))
		{
			Film.Load(p_parser);
		}
		if(p_parser->FindWord("image {"))
		{
			Image.Load(p_parser);
		}
		if(p_parser->FindWord("lighting {"))
		{
			Lighting.Load(p_parser);
		}
		if(p_parser->FindWord("description {"))
		{
			while( p_parser->Length()!=(1+LINERETURN))
			{
				p_parser->ReadLine();
			}
		}
		p_parser->ReadLine();
	}
	return true;
}

