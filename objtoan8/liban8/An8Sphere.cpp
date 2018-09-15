// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Sphere::An8Sphere(void)
{
	AN8XMatrixIdentity(&Matrix);
	BaseOrigin       = AN8XVECTOR3(0.0f,0.0f,0.0f);         // Location
	BaseOrientation  = AN8XQUATERNION(0.0f,0.0f,0.0f,1.0f); // Orientation (quaternion)

	Kind = 0;  // 0=normal, 1=geodesic

	MaterialID = 0;

	Diameter = 0.0f;
	DivLong = 1;
	DivLat = 1;
	Geodesic = 1;
}

An8Sphere::~An8Sphere(void)
{

}

void An8Sphere::Load(AN8XPARSER*p_parser)
{
	////Name/////////////
	p_parser->ReadLine();
	p_parser->GetName(&Name);

	////Base Origin////
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
				BaseOrigin.x = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,""," ");
				BaseOrigin.y = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,"",")");
				BaseOrigin.z = (float) atof( element.c_str());
			}
			if(p_parser->FindWord("orientation {"))
			{
				std::string strLine = p_parser->GetLine();
				p_parser->CutStr(&strLine,"(",1);
				string element = p_parser->SubStr(&strLine,""," ");
				BaseOrientation.x = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,""," ");
				BaseOrientation.y = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,""," ");
				BaseOrientation.z = (float) atof( element.c_str());

				p_parser->CutStr(&strLine," ",1);
				element = p_parser->SubStr(&strLine,"",")");
				BaseOrientation.w = (float) atof( element.c_str());
			}
			p_parser->ReadLine();
			baseEnd = p_parser->IndexOf(1,"}");
		} // while
		p_parser->ReadLine();
	}
	

	// material//////
	p_parser->GetName(&Material);

	// diameter////
	p_parser->ReadLine();

	p_parser->CutLine("{",1);
	string element = p_parser->SubStr(" "," ");
	Diameter = (float)atof( element.c_str());

	// longlat///
	p_parser->ReadLine();
	if(p_parser->FindWord("longlat {"))
	{
		Kind=0;

		p_parser->CutLine("{",2);
		string element = p_parser->SubStr(""," ");
		DivLong = atoi( element.c_str());

		p_parser->CutLine(" ",1);
		element = p_parser->SubStr(""," ");
		DivLat = atoi( element.c_str());
	}
	else
	{
		Kind=1;

		p_parser->CutLine("{",1);
		string element = p_parser->SubStr(" "," ");
		Geodesic = atoi( element.c_str());

	}

	p_parser->ReadLine();

}

void An8Sphere::ComputeMaterialID(std::vector<An8Material>& i_vMaterials)
{
	for(int material = 0 ;material< (int) i_vMaterials.size();++material)
	{
		if( i_vMaterials[material].Name.compare(Material) == 0 )
		{
				MaterialID = material;
				break;
		}
		//else   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//{
		//}
	}
}


//////////////////////////////////////////////////////////
//	Compute Matrix							       	    //
//////////////////////////////////////////////////////////
void An8Sphere::ComputeMatrix(bool includeTranslation)
{
			AN8XMATRIX matrixTmp;
			AN8XMatrixRotationQuaternion( &matrixTmp , &BaseOrientation );
			if( includeTranslation == true )
			{
				matrixTmp[12] = BaseOrigin.x;
				matrixTmp[13] = BaseOrigin.y;
				matrixTmp[14] = BaseOrigin.z;
			}

			AN8XMatrixCopy( &Matrix , &matrixTmp);
}


void An8Sphere::ComputePosition(AN8XQUATERNION *i_pBaseOrientation,AN8XVECTOR3* i_pBaseOrigin)
{
	if(i_pBaseOrientation != NULL && i_pBaseOrigin != NULL)
	{
		/*
		AN8XQuaternionMultiply(&BaseOrientation,&BaseOrientation,i_pBaseOrientation);
		BaseOrigin.x += i_pBaseOrigin->x;
		BaseOrigin.y += i_pBaseOrigin->y;
		BaseOrigin.z += i_pBaseOrigin->z;
		*/
		/*
		// begin new
		AN8XQuaternionMultiply(&BaseOrientation,&BaseOrientation,i_pBaseOrientation);
		AN8XVECTOR4 vec4;AN8XMATRIX matrix;
		AN8XMatrixRotationQuaternion(&matrix,i_pBaseOrientation);
		AN8XVec3Transform(&vec4,&BaseOrigin,&matrix);
		BaseOrigin.x = i_pBaseOrigin->x + vec4.x;
		BaseOrigin.y = i_pBaseOrigin->y + vec4.y;
		BaseOrigin.z = i_pBaseOrigin->z + vec4.z;
		*/
		AN8XBaseConcatenate(
					&BaseOrigin, &BaseOrientation,
					i_pBaseOrigin, i_pBaseOrientation,
					&BaseOrigin, &BaseOrientation);
	}
}

void An8Sphere::ComputeBoundingElements(void)
{
	BoundingBox.Min.x = BaseOrigin.x - (Diameter / 2.0f); 
	BoundingBox.Min.y = BaseOrigin.y - (Diameter / 2.0f); 
	BoundingBox.Min.z = BaseOrigin.z - (Diameter / 2.0f); 

	BoundingBox.Max.x = BaseOrigin.x + (Diameter / 2.0f); 
	BoundingBox.Max.y = BaseOrigin.y + (Diameter / 2.0f); 
	BoundingBox.Max.z = BaseOrigin.z + (Diameter / 2.0f); 
}
