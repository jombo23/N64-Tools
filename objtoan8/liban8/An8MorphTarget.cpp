// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8MorphTarget::An8MorphTarget(void)
{

}

An8MorphTarget::~An8MorphTarget(void)
{

}


void An8MorphTarget::Load(AN8XPARSER*p_parser)
{
	p_parser->ReadLine();

	if(p_parser->FindWord("name {"))
		p_parser->GetName(&Name);

}
