// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8NamedObject::An8NamedObject(void)
{
	NbrWeightedBy = 0;
	NbrWeights = 0;
}

An8NamedObject::~An8NamedObject(void)
{
}

void An8NamedObject::Load(AN8XPARSER*p_parser)
{
	int elementStart = p_parser->IndexOf(1,"namedobject {");

	An8Weights newWeights;

	// Name
	p_parser->GetName(&ObjectName);

	p_parser->ReadLine();
	if(p_parser->FindWord("name {"))
	{
		p_parser->GetName(&Name);
	}
	else{throw "name { not found in namedobject chunk.";}

	p_parser->ReadLine();
	if(p_parser->FindWord("base {"))
	{
		int baseStart = p_parser->IndexOf(1,"base {");
		int baseEnd = p_parser->IndexOf(1,"}");

		while( p_parser->IndexOf(1, "}")!= 0 && baseStart != baseEnd)
		{
			if(p_parser->FindWord("origin {"))
			{
				std::string strLine = p_parser->GetLine();
				p_parser->CutStr(&strLine,"(",1);
				string element = p_parser->SubStr(&strLine,""," ");
				Origin.x = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,""," ");
				Origin.y = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,"",")");
				Origin.z = (float) atof( element.c_str());
			}
			if(p_parser->FindWord("orientation {"))
			{
				std::string strLine = p_parser->GetLine();
				p_parser->CutStr(&strLine,"(",1);
				string element = p_parser->SubStr(&strLine,""," ");
				Orientation.x = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,""," ");
				Orientation.y = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,""," ");
				Orientation.z = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,"",")");
				Orientation.w = (float) atof( element.c_str());
			}
			p_parser->ReadLine();
			baseEnd = p_parser->IndexOf(1,"}");
		}
	}

	p_parser->ReadLine();

	if(p_parser->FindWord("material {"))
	{
		p_parser->GetName(&Material);
		p_parser->ReadLine();
	}
	while(p_parser->FindWord("weightedby {") )
	{
		++NbrWeightedBy;
		string str;
		p_parser->GetName(&str);
		vWeightedBy.push_back(str);
		p_parser->ReadLine();
	}

	int elementEnd = p_parser->IndexOf(1,"}");
	while(p_parser->Length()!=1+LINERETURN && elementStart != elementEnd)
	{
		if(p_parser->FindWord("weights {"))
		{
			++NbrWeights;
			vWeights.push_back(newWeights);
			vWeights[vWeights.size()-1].Load(p_parser);
		}

		p_parser->ReadLine();
		elementEnd =p_parser->IndexOf(1,"}");
	}

}
