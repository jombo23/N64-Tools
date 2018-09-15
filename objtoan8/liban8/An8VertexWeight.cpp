// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8VertexWeight::An8VertexWeight(void)
{
	NbrOfWeightValue = 0;
}

An8VertexWeight::~An8VertexWeight(void)
{

}

void An8VertexWeight::Load(std::string strWeight)
{
	int endpos = (int) strWeight.find(" ",0);
	string element = strWeight.substr(0,endpos);
	NbrOfWeightValue = atoi( element.c_str() );

	string str = strWeight;
	for( int i = 0 ; i < NbrOfWeightValue ; ++i )
	{
		endpos = (int)str.find("(",0);
		str = str.substr(endpos+1,str.length()-endpos-1);

		endpos = (int)str.find(" ",0);
		element = str.substr(0,endpos);
		An8WeightValue newWeightValue;
		newWeightValue.BoneIndex = atoi( element.c_str() );

		str = str.substr(endpos+1,str.length()-endpos-1);
		endpos = (int)str.find(")",0);
		element = str.substr(0,endpos);
		newWeightValue.Weight = (float)atof( element.c_str() );

		vWeightValues.push_back(newWeightValue);

	}
}


