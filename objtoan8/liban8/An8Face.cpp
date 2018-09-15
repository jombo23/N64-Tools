// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Face::An8Face(void)
{
	NbrVertex = 0;  // Number of vertices in the face
	Mask = 0;      // Mask
	Material = 0;    // Material ID
	NormalOption = -1; // Normal option

	//vector<An8FaceIndice>	vIndices;  // Vector of indices
	vIndices.reserve(3);
	Normal = AN8XVECTOR3(0.0f,1.0f,0.0f);  // Face normal

	//vertexNormal[0] = An8v3D(0.0f,1.0f,0.0f);
	//vertexNormal[1] = An8v3D(0.0f,1.0f,0.0f);
	//vertexNormal[2] = An8v3D(0.0f,1.0f,0.0f);

	vertexTangents[0] = AN8XVECTOR3(0.0f,1.0f,0.0f);
	vertexTangents[1] = AN8XVECTOR3(0.0f,1.0f,0.0f);
	vertexTangents[2] = AN8XVECTOR3(0.0f,1.0f,0.0f);

	vertexBinormals[0] = AN8XVECTOR3(0.0f,1.0f,0.0f);
	vertexBinormals[1] = AN8XVECTOR3(0.0f,1.0f,0.0f);
	vertexBinormals[2] = AN8XVECTOR3(0.0f,1.0f,0.0f);
}

An8Face::~An8Face(void)
{

}

bool An8Face::HasVertexNormal(void)
{
	if( Mask == 2 || Mask == 3 || Mask == 6 || Mask == 7 )
		return true;
	return false;
}

bool An8Face::HasTexCoord(void)
{
	if( Mask == 4 || Mask == 5 || Mask == 6 || Mask == 7 )
		return true;
	return false;
}

bool An8Face::Load(AN8XPARSER*p_parser)
{
	int i,j;
	i=1;j=0;

	int countBegin,countEnd;

	int NbIndices=0;

	An8FaceIndice newIndices;

	int IsNewFace=1;
	int StateOfFaceLine=1;
	int FaceLineFinished=0;

	countBegin =p_parser->CountChar( "(" );
	countEnd   =p_parser->CountChar( ")" );

	//////////////Début de la lecture///////////////////////////
	do
	{
		///// a new face ?/////////////////////
		if(IsNewFace)
		{
			// we add one face
			sscanf(p_parser->GetLine(),"%d %d %d %d\n",
					&NbrVertex,
					&Mask,
					&Material,
					&NormalOption

					);
			NbIndices=0;
			IsNewFace=0;

			if(countBegin==countEnd) // a new face with in 1 line
				StateOfFaceLine=1;
			else if(countBegin>countEnd) // a new face with in 2 line
			{StateOfFaceLine=2;}

		}
		else{   // this is not a new face

			if(countBegin==countEnd) // an intermediate line
				StateOfFaceLine=3;
			else if(countBegin<countEnd) // the last line of the face
				StateOfFaceLine=4;
		}

			do{
				FaceLineFinished=0;

				if(countBegin!=0)  // the line is just ')'
				{
					while( p_parser->GetLine()[i]!='(' && i<p_parser->Length()) ++i;	// go to the first '('

					if( (countBegin==countEnd && StateOfFaceLine==1) || (countBegin>countEnd && StateOfFaceLine==2))		// the first '(' is not for the first indice
					{	++i;
						while(p_parser->GetLine()[i]!='(') ++i;
						--countBegin;
					}

					p_parser->SubLine(i,p_parser->Length());

					// we add one indice structure
					vIndices.push_back(newIndices);

					// we scan it
					///////////////////Texture and Normals coordinates present/////////////////////////////////////
					if( Mask == 6 || Mask == 7 )
					{
						sscanf(p_parser->GetLine(),"(%d %d %d\n",
						&vIndices[NbIndices].Point,
						&vIndices[NbIndices].Normal,
						&vIndices[NbIndices].TexCoord
						);

					}///////////////////Normals coordinates present (but not texture)/////////
					else if( Mask == 2 )
					{
						sscanf(p_parser->GetLine(),"(%d %d\n",
							&vIndices[NbIndices].Point,
							&vIndices[NbIndices].Normal
						);
						vIndices[NbIndices].TexCoord=0;
					}
					///////////////////Texture coordinates present/////////////////////////////////////
					else if( Mask >= 4 )
					{
						sscanf(p_parser->GetLine(),"(%d %d\n",
							&vIndices[NbIndices].Point,
							&vIndices[NbIndices].TexCoord
						);
					}else /////////////No Texture coordinates present///////////////////////////////
					{
						// we scan it
						sscanf(p_parser->GetLine(),"(%d\n",
							&vIndices[NbIndices].Point
						);
						vIndices[NbIndices].TexCoord=0;
					}

					++NbIndices;
					--countBegin;
					--countEnd;

					if(StateOfFaceLine==1 && countBegin==0) FaceLineFinished=1;
					else if(StateOfFaceLine==2 && countBegin==0) FaceLineFinished=1;
					else if(StateOfFaceLine==3 && countBegin==0) FaceLineFinished=1;
					else if(StateOfFaceLine==4 && countBegin==0) FaceLineFinished=1;


				}
				else
				{
					FaceLineFinished=1;
				}
				j=0;i=1;

			}while(!FaceLineFinished);

		if(StateOfFaceLine==1 || StateOfFaceLine==4)
			IsNewFace=1;

		if(IsNewFace==0) // we have not finished with this face
			p_parser->ReadLine();

		countBegin =p_parser->CountChar( "(" );
		countEnd   =p_parser->CountChar( ")" );

	}while(IsNewFace==0);

	return true;
}
