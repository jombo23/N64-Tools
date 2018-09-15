// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Weights::An8Weights(void)
{
	NbrOfVertexWeight = 0;
}

An8Weights::~An8Weights(void)
{

}


void An8Weights::Load(AN8XPARSER*p_parser)
{
	An8VertexWeight newVertexWeight;

	p_parser->GetName(&MeshName);
	p_parser->ReadLine();

	std::string strWeights = "";
	while(p_parser->FindWord("(") || p_parser->FindWord(")"))
	{
		strWeights += p_parser->GetLine();
		p_parser->ReadLine();
	}

	// A finir
	// Separer les An8VertexWeight et les donner en argument au An8VertexWeight

	//p_parser->SetLine(strWeights);
	//while( p_parser->FindWord("(") )
	string element;
	while( strWeights.find("(",0) != std::string::npos)
	{
		++NbrOfVertexWeight;
		
		vVertexWeights.push_back(newVertexWeight);

		element = p_parser->SubStr(&strWeights,"(","))");
		vVertexWeights[vVertexWeights.size()-1].Load(element+")");

		p_parser->CutStr(&strWeights,"))",2);
		
	}


}
