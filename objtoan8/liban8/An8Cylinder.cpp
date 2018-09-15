// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;
#include<math.h>

An8Cylinder::An8Cylinder(void)
{
	AN8XMatrixIdentity(&Matrix);

	BaseOrigin = AN8XVECTOR3(0.0f,0.0f,0.0f);         // Location
	BaseOrientation  = AN8XQUATERNION(0.0f,0.0f,0.0f,1.0f); // Orientation (quaternion)

	MaterialID = 0;

	Length = 0.0f;
	StartDiameter = 0.0f;
	EndDiameter = 0.0f;
	DivLong = 1;
	DivLat = 1;
	CapStart = false;
	CapEnd = false;
}

An8Cylinder::~An8Cylinder(void)
{

}

void An8Cylinder::Load(AN8XPARSER*p_parser)
{
	////Name/////////////
	p_parser->ReadLine();
	p_parser->GetName(&Name);

	////Base Origin////:
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

	// length////
	p_parser->ReadLine();

	p_parser->CutLine("{",1);
	string element = p_parser->SubStr(" "," ");
	Length = (float) atof( element.c_str());

	// diameter////
	p_parser->ReadLine();

	p_parser->CutLine("{",1);
	element = p_parser->SubStr(" "," ");
	StartDiameter = (float) atof( element.c_str());

	// topdiameter////
	p_parser->ReadLine();

	p_parser->CutLine("{",1);
	element = p_parser->SubStr(" "," ");
	EndDiameter = (float) atof( element.c_str());

	// longlat///
	p_parser->ReadLine();

	p_parser->CutLine("{",2);
	element = p_parser->SubStr(""," ");
	DivLong = atoi( element.c_str());

	p_parser->CutLine(" ",1);
	element = p_parser->SubStr(""," ");
	DivLat =  atoi( element.c_str());

	// Capstart//
	CapStart = false;
	CapEnd   = false;

	p_parser->ReadLine();

	if(p_parser->FindWord("capend {") ||p_parser->FindWord("capstart {"))
	{
		if(p_parser->FindWord("capend {"))
			CapEnd = true;

		if(p_parser->FindWord("capstart {"))
			CapStart = true;

		p_parser->ReadLine();
	}

	if(p_parser->FindWord("capend {") ||p_parser->FindWord("capstart {"))
	{
		if(p_parser->FindWord("capend {"))
			CapEnd=true;

		if(p_parser->FindWord("capstart {"))
			CapStart=true;

		p_parser->ReadLine();
	}


}

void An8Cylinder::ComputeMaterialID(std::vector<An8Material>& i_vMaterials)
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
void An8Cylinder::ComputeMatrix(bool includeTranslation)
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

void An8Cylinder::ComputePosition(AN8XQUATERNION *i_pBaseOrientation,AN8XVECTOR3* i_pBaseOrigin)
{
	if(i_pBaseOrientation != NULL && i_pBaseOrigin != NULL)
	{
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


void An8Cylinder::ComputeBoundingElements(void)
{
	BoundingBox.Min.x = - (StartDiameter/2.0f);
	BoundingBox.Min.y = 0.0f;
	BoundingBox.Min.z = - (StartDiameter/2.0f);

	BoundingBox.Max.x = (EndDiameter/2.0f);
	BoundingBox.Max.y =  Length;
	BoundingBox.Max.z = (EndDiameter/2.0f);

	AN8XMoveBoundingBox(&BoundingBox,&BaseOrigin,&BaseOrientation);

	BoundingSphereCenter.x = (BoundingBox.Min.x+BoundingBox.Max.x)/2.0f; // X
	BoundingSphereCenter.y = (BoundingBox.Min.y+BoundingBox.Max.y)/2.0f; // Y
	BoundingSphereCenter.z = (BoundingBox.Min.z+BoundingBox.Max.z)/2.0f; // Z

	BoundingSphereRadius =
		 (BoundingBox.Max.x - BoundingBox.Min.x)*(BoundingBox.Max.x - BoundingBox.Min.x)
		+(BoundingBox.Max.y - BoundingBox.Min.y)*(BoundingBox.Max.y - BoundingBox.Min.y)
		+(BoundingBox.Max.z - BoundingBox.Min.z)*(BoundingBox.Max.z - BoundingBox.Min.z);

	BoundingSphereRadius = sqrt(BoundingSphereRadius);
	BoundingSphereRadius /= 2.0f;
}
