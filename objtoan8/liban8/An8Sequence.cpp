// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Sequence::An8Sequence(void)
{
	NbrFrame = 0;
	NbrJointAngle = 0;
}

An8Sequence::~An8Sequence(void)
{

}


bool An8Sequence::Load(AN8XPARSER*p_parser)
{

	while( /*p_parser->Length()!=1+LINERETURN*/ p_parser->IndexOf(1, "}")!= 0   ) // While we do not find the end of the object
	{
		if(p_parser->FindWord("sequence {"))
		{
			p_parser->GetName(&Name);
		}
		else if(p_parser->FindWord("figure {"))
		{
			p_parser->GetName(&FigureName);
		}
		else if(p_parser->FindWord("frames {"))
		{
			p_parser->CutLine("{",1);
			string element = p_parser->SubStr(" "," ");
			NbrFrame = atoi( element.c_str());
		}
		else if(p_parser->FindWord("jointangle {"))
		{
			++NbrJointAngle;
			An8JointAngle newJointAngle; 
			vJointAngles.push_back(newJointAngle);
			vJointAngles[vJointAngles.size()-1].Load(p_parser);	
		}
		p_parser->ReadLine();
	}

	
	return true;
}

