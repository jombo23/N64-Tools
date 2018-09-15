// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Header::An8Header(void)
{

}

An8Header::~An8Header(void)
{

}

////////////////////////////////
// Load the header
///////////////////////////////
bool An8Header::Load(AN8XPARSER*p_parser)
{

   //p_parser->ReadLine();
		
	if(p_parser->FindWord( "header"))
	{
	 p_parser->ReadLine();
	 p_parser->GetName( &Version);
     p_parser->ReadLine();
     p_parser->GetName( &Build);
	 p_parser->ReadLine();
    }
    else
      return false;
		
	return true;
}

