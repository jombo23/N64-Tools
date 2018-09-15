// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8JointAngle::An8JointAngle(void)
{
	Axe = 0;
	HasTrack = false;
}

An8JointAngle::~An8JointAngle(void)
{

}


bool An8JointAngle::Load(AN8XPARSER*p_parser)
{
	string element;
	p_parser->Get2Names(&BoneName,&element);
	
	if( element.compare("X") == 0 )
		Axe = 1;

	if( element.compare("Y") == 0 )
		Axe = 2;

	if( element.compare("Z") == 0 )
		Axe = 3;

	int jointAngleStart =p_parser->IndexOf(1,"jointangle {");
	if(jointAngleStart<0) return false;
	int jointAngleEnd =p_parser->IndexOf(1,"}");

	while(p_parser->Length()!=1+LINERETURN && jointAngleStart != jointAngleEnd) 
	{
		if(p_parser->FindWord("track {"))
		{
			HasTrack = true;
			Track.Load(p_parser);
		}
		p_parser->ReadLine();
	    jointAngleEnd =p_parser->IndexOf(1,"}");
	}
	

	return true;
}

