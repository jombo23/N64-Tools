// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
#include <math.h>
using namespace std;

An8Mesh::An8Mesh(void)
{
	AN8XMatrixIdentity(&Matrix);

	BaseOrigin = AN8XVECTOR3(0.0f,0.0f,0.0f);         // Location
	BaseOrientation  = AN8XQUATERNION(0.0f,0.0f,0.0f,1.0f); // Orientation (quaternion)

	IsSubdivision = 0;  // = 0 if no, = 1 if yes
    Working = 0;
    Divisions = 0;

	SmoothAngle = 60;
	HasMatrial = 0 ; // = 0 if no, = 1 if yes

	NbrPoint = 0;
	NbrEdge = 0;
	NbrFace = 0;
	NbrNormal = 0;
	NbrUVCoord = 0;

	NbrMorphOffsets = 0;

	NbrMateriallist = 0 ;

	NbrBinormal = 0;

	HasTexGen = false;
}

An8Mesh::~An8Mesh(void)
{

}



void An8Mesh::Load(AN8XPARSER*p_parser,An8Object* object)
{
  try{
	IsSubdivision=0;

	An8MorphOffsets newMorphOffsets;


	if(p_parser->FindWord("subdivision {"))
	{
		IsSubdivision=1;
	}

	////Name/////////////
	p_parser->ReadLine();
	p_parser->GetName(&Name);

	while(!p_parser->FindWord("materiallist {") && p_parser->ReadLine()!=0 )
	{
		////Base Origin////:
		if(p_parser->FindWord("base {"))
		{
			// BaseOrigin
			p_parser->ReadLine();
			if(p_parser->FindWord("origin {"))
			{
				sscanf(p_parser->GetLine(),"      origin { (%f %f %f\n",
							&BaseOrigin.x,
							&BaseOrigin.y,
							&BaseOrigin.z
				);
			}

			// BaseOrientation
			p_parser->ReadLine();
			if(p_parser->FindWord("orientation {"))
			{
				sscanf(p_parser->GetLine(),"      orientation { (%f %f %f %f\n",
							&BaseOrientation.x,
							&BaseOrientation.y,
							&BaseOrientation.z,
							&BaseOrientation.w

				);
			}
		}

		////material/////////////
		if(p_parser->FindWord("material {"))
		{
			p_parser->GetName(&Material);
		}

		// Smoothangle
		if(p_parser->FindWord("smoothangle {"))
		{
			sscanf(p_parser->GetLine(),"    smoothangle { %d\n",&SmoothAngle);
		}

		// Texgen
		if(p_parser->FindWord("texgen {"))
		{
			HasTexGen = true;
			TexGen.Load(p_parser);
		}
		// Subdivision
		if(p_parser->FindWord("working {"))
		{
			sscanf(p_parser->GetLine(),"    working { %d\n",&Working);
		}
		if(p_parser->FindWord("divisions {"))
		{
			sscanf(p_parser->GetLine(),"    divisions { %d\n",&Divisions);
		}

		////points,faces,uvCoords/////
		if( p_parser->FindWord("/*") && p_parser->FindWord("points,") && p_parser->FindWord("faces,") && p_parser->FindWord("uvCoords") && p_parser->FindWord("*/"))
		{
			sscanf(p_parser->GetLine(),"    /* %d points, %d faces, %d uvCoords */\n",
				&NbrPoint,
				&NbrFace,
				&NbrUVCoord
			);
		}

		///Edges? ////////
		if(p_parser->FindWord("special edges"))
		{
			sscanf(p_parser->GetLine(),"    /* %d special edges */\n",
				&NbrEdge);
		}

	} // While no materiallist {







	////materiallist//////////////
	if(p_parser->FindWord("materiallist {"))
	{
		ReadMateriallist(p_parser,object);
	}
	else
	{
		throw "materiallist { not found in mesh chunk.";
	}
	// We are in the and of the chunk: "}"

	////points////////////////////
	if(NbrPoint>0)
	{// Some meshes can be empty
		p_parser->ReadLine();
		while(!p_parser->FindWord("points {"))
			{p_parser->ReadLine();}
		ReadPoints(p_parser);
	}

	////////////////////
	p_parser->ReadLine();

	//// 1st Test: Edge, Normal or TextCoord////////////////////
	if(p_parser->FindWord("edges {") ||p_parser->FindWord("normals {") ||p_parser->FindWord("texcoords {"))
	{
		if(p_parser->FindWord("edges {"))
		{
			ReadAn8Edges(p_parser);
			p_parser->ReadLine();
		}
		else if(p_parser->FindWord("normals {"))
		{
			//HasNormal=true;
			ReadAn8Normals(p_parser);
			p_parser->ReadLine();
		}
		else if(p_parser->FindWord("texcoords {"))
		{
			ReadTexCoords(p_parser);
			p_parser->ReadLine();
		}
	}

	//// 2nd Test: Edge, Normal or TextCoord////////////////////
	if(p_parser->FindWord("edges {") ||p_parser->FindWord("normals {") ||p_parser->FindWord("texcoords {"))
	{
		if(p_parser->FindWord("edges {"))
		{
			ReadAn8Edges(p_parser);
			p_parser->ReadLine();
		}
		else if(p_parser->FindWord("normals {"))
		{
			//HasNormal=true;
			ReadAn8Normals(p_parser);
			p_parser->ReadLine();
		}
		else if(p_parser->FindWord("texcoords {"))
		{
				ReadTexCoords(p_parser);
				p_parser->ReadLine();
		}
	}

	//// 3rd Test: Edge, Normal or TextCoord////////////////////
	if(p_parser->FindWord("edges {") ||p_parser->FindWord("normals {") ||p_parser->FindWord("texcoords {"))
	{
		if(p_parser->FindWord("edges {"))
		{
			ReadAn8Edges(p_parser);
			p_parser->ReadLine();
		}
		else if(p_parser->FindWord("normals {"))
		{
			//HasNormal=true;
			ReadAn8Normals(p_parser);
			p_parser->ReadLine();
		}
		else if(p_parser->FindWord("texcoords {"))
		{
			ReadTexCoords(p_parser);
			p_parser->ReadLine();
		}
	}

	////Faces////////////////////
	if(p_parser->FindWord("faces {"))
	{
		p_parser->ReadLine(); // first face

		An8Face newFace;
		vFaces.reserve(this->NbrFace);
		while(!p_parser->FindWord("}"))
		{
			vFaces.push_back(newFace);
			vFaces[vFaces.size()-1].Load(p_parser);
			p_parser->ReadLine();
		}
		p_parser->ReadLine(); // TODO: improve
	}






	////morphoffsets////////////////////
	// ???
	while(p_parser->FindWord("morphoffsets {"))
	{
		if(p_parser->FindWord("morphoffsets {"))
		{
			++NbrMorphOffsets;
			vMorphOffsets.push_back(newMorphOffsets);
			vMorphOffsets[vMorphOffsets.size()-1].Load(p_parser);
		}
		p_parser->ReadLine();
	}

  }catch(char* msg)
  {
    throw msg;
  }
}


void An8Mesh::ReadMateriallist(AN8XPARSER*p_parser,An8Object* object)
{
	string Temp_MatName;
	int i_material;
	int tempNbrMateriallist;

	bool matFound = false;
	tempNbrMateriallist=0;

	p_parser->ReadLine();
	p_parser->GetName(&Temp_MatName);

	while(p_parser->FindWord("materialname {"))
	{
		for(i_material=0;i_material<object->NbrMaterial;++i_material)
		{
			//if( strcmp(object->vMaterials[i_material].Name,Temp_MatName)==0 )
			if( object->vMaterials[i_material].Name.compare(Temp_MatName) == 0 ) // GSMX
			{
				vMatNumber.push_back(i_material);
				vMatNumber[tempNbrMateriallist]=i_material;
				tempNbrMateriallist+=1;
				matFound =true;
			}
			//else   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			//{
			//}
		}
        if(matFound==false)
		{
          vMatNumber.push_back(i_material);
		  vMatNumber[tempNbrMateriallist]= object->NbrMaterial-1;
	      tempNbrMateriallist+=1;
		}
		matFound = false;

		p_parser->ReadLine();
		p_parser->GetName(&Temp_MatName);
	}

	NbrMateriallist=tempNbrMateriallist;
}

void An8Mesh::ReadPoints(AN8XPARSER*p_parser)
{
	int i,j;

	int countchar;
	int NbPoints=0;
	AN8XVECTOR3 newPoint;
	string element;
	string str;
	int start = 0;
	int end   = 0;

	vPoints.reserve(this->NbrPoint);

	i=0;j=0;
	p_parser->ReadLine();		// First line = first face
	countchar=p_parser->CountChar( "(" );

	while(countchar>0)
	{
			str = p_parser->GetLine();
			start = 0;
			end   = 0;
			
			while(  (start = (int)str.find("(",0)) != (int)std::string::npos )
			{
				vPoints.push_back(newPoint);

				str = str.substr(start,str.length());
				end   = (int)str.find(" ",0);
				element = str.substr(1,end-1);
				vPoints[NbPoints].x = (float) atof ( element.c_str() );

				str = str.substr(end+1,str.length());
				end   = (int)str.find(" ",0);
				element = str.substr(0,end);
				vPoints[NbPoints].y = (float) atof ( element.c_str() );

				str = str.substr(end+1,str.length());
				end   = (int)str.find(" ",0);
				element = str.substr(0,end-1);
				vPoints[NbPoints].z = (float) atof ( element.c_str() );

				str = str.substr(end+1,str.length());

				NbPoints=NbPoints+1;
			}
		///
		i=0;
		p_parser->ReadLine();
		countchar=p_parser->CountChar( "(" );
	}

}


void An8Mesh::ReadAn8Edges(AN8XPARSER*p_parser)
{
	int i,j;

	int countchar;
	int nbEdges=0;
	An8vpi2D newEdge;
	string strTemp;

	i=0;j=0;

	p_parser->ReadLine();
	countchar=p_parser->CountChar( "(" );

	while(countchar>0)
	{
		while(  p_parser->CountChar("(") > 0 )
		{
			vEdges.push_back(newEdge);

			p_parser->CutLine("(",0);
			strTemp = p_parser->SubStr("("," ");
			vEdges[nbEdges].P1 = atoi ( strTemp.c_str() );

			p_parser->CutLine(" ",0);
			strTemp = p_parser->SubStr(" ",")");
			vEdges[nbEdges].P2 = atoi ( strTemp.c_str() );

			nbEdges=nbEdges+1;
		}
		///
		i=0;
		p_parser->ReadLine();
		countchar =p_parser->CountChar( "(" );
	}

	this->NbrEdge=nbEdges;

}

void An8Mesh::ReadAn8Normals(AN8XPARSER*p_parser)
{
	int i,j;

	int countchar;
	int NbNormals=0;
	AN8XVECTOR3 newNormal;
	string strTemp;

	i=0;j=0;

	p_parser->ReadLine();
	countchar =p_parser->CountChar( "(" );
	while(countchar>0)
	{
		while(  p_parser->CountChar("(") > 0 )
		{
			vNormals.push_back(newNormal);

			p_parser->CutLine("(",0);
			strTemp = p_parser->SubStr("("," ");
			vNormals[NbNormals].x = (float)atof ( strTemp.c_str() );

			p_parser->CutLine(" ",0);
			strTemp = p_parser->SubStr(" "," ");
			vNormals[NbNormals].y = (float)atof ( strTemp.c_str() );

			p_parser->CutLine(" ",1);
			strTemp = p_parser->SubStr(" ",")");
			vNormals[NbNormals].z = (float)atof ( strTemp.c_str() );

			NbNormals=NbNormals+1;
		}
		///
		i=0;
		p_parser->ReadLine();
		countchar=p_parser->CountChar( "(" );
	}

	NbrNormal=NbNormals;
}

void An8Mesh::ReadAn8Binormals(AN8XPARSER*p_parser)
{
	int i,j;

	int countchar;
	int nbrBinormal = 0;
	AN8XVECTOR3 newBinormal;
	string strTemp;

	i=0;j=0;

	p_parser->ReadLine();
	countchar =p_parser->CountChar( "(" );
	while(countchar>0)
	{
		while(  p_parser->CountChar("(") > 0 )
		{
			vBinormals.push_back(newBinormal);

			p_parser->CutLine("(",0);
			strTemp = p_parser->SubStr("("," ");
			vBinormals[nbrBinormal].x = (float)atof ( strTemp.c_str() );

			p_parser->CutLine(" ",0);
			strTemp = p_parser->SubStr(" "," ");
			vBinormals[nbrBinormal].y = (float)atof ( strTemp.c_str() );

			p_parser->CutLine(" ",1);
			strTemp = p_parser->SubStr(" ",")");
			vBinormals[nbrBinormal].z = (float)atof ( strTemp.c_str() );

			nbrBinormal = nbrBinormal+1;
		}
		///
		i=0;
		p_parser->ReadLine();
		countchar=p_parser->CountChar( "(" );
	}

	NbrBinormal = nbrBinormal;
}


void An8Mesh::ReadTexCoords(AN8XPARSER*p_parser)
{
	int i,j;

	int countchar;
	int NbTexCoords=0;
	AN8XVECTOR2 newTexCoord;
	vTexCoords.reserve(this->NbrUVCoord);

	i=1;j=0;

	p_parser->ReadLine();
	countchar=p_parser->CountChar( "(" );

	string strTemp;
	while(countchar>0)
	{
		while(  p_parser->CountChar("(") > 0 )
		{
			vTexCoords.push_back(newTexCoord);

			p_parser->CutLine("(",0);
			strTemp = p_parser->SubStr("("," ");
			vTexCoords[NbTexCoords].x = (float)atof ( strTemp.c_str() );

			p_parser->CutLine(" ",0);
			strTemp = p_parser->SubStr(" ",")");
			vTexCoords[NbTexCoords].y = (float)atof ( strTemp.c_str() );

			NbTexCoords=NbTexCoords+1;
		}
		///
		i=0;
		p_parser->ReadLine();
		countchar =p_parser->CountChar( "(" );
	}


}






void An8Mesh::Triangulize(void)
{
	int lcFaceLoop;

	int lcFace2Create = 0;
	int newNbrFace = 0;

	lcFaceLoop = NbrFace;

	vector<An8Face>	vNewFaces;
	An8Face newFace;
	An8FaceIndice newFaceIndex;

	while(lcFaceLoop>0) // for all faces
	{
		if( vFaces[lcFaceLoop-1].NbrVertex > 3 )
		{
			lcFace2Create= vFaces[lcFaceLoop-1].NbrVertex-3;

			while(lcFace2Create>0)
			{
				++newNbrFace;

				// Create new face
				vNewFaces.push_back(newFace);

				--lcFace2Create;

				vNewFaces[vNewFaces.size()-1].NbrVertex   = 3 ;
				vNewFaces[vNewFaces.size()-1].Mask         = vFaces[lcFaceLoop-1].Mask;
				vNewFaces[vNewFaces.size()-1].Material     = vFaces[lcFaceLoop-1].Material;
				vNewFaces[vNewFaces.size()-1].NormalOption = vFaces[lcFaceLoop-1].NormalOption;

				vNewFaces[vNewFaces.size()-1].vIndices.push_back(newFaceIndex);
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].Point    = vFaces[lcFaceLoop-1].vIndices[0].Point;
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].Normal	 = vFaces[lcFaceLoop-1].vIndices[0].Normal;
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].TexCoord = vFaces[lcFaceLoop-1].vIndices[0].TexCoord;

				vNewFaces[vNewFaces.size()-1].vIndices.push_back(newFaceIndex);
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].Point    = vFaces[lcFaceLoop-1].vIndices[lcFace2Create+2].Point;
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].Normal	 = vFaces[lcFaceLoop-1].vIndices[lcFace2Create+2].Normal;
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].TexCoord = vFaces[lcFaceLoop-1].vIndices[lcFace2Create+2].TexCoord;

				vNewFaces[vNewFaces.size()-1].vIndices.push_back(newFaceIndex);
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].Point    = vFaces[lcFaceLoop-1].vIndices[lcFace2Create+3].Point;
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].Normal	 = vFaces[lcFaceLoop-1].vIndices[lcFace2Create+3].Normal;
				vNewFaces[vNewFaces.size()-1].vIndices[vNewFaces[vNewFaces.size()-1].vIndices.size()-1].TexCoord = vFaces[lcFaceLoop-1].vIndices[lcFace2Create+3].TexCoord;
				
			} // End of while(lcFace2Create>0)

			// Update original face
			lcFace2Create = vFaces[lcFaceLoop-1].NbrVertex - 3 ;
			vFaces[lcFaceLoop-1].NbrVertex = 3;
			while(lcFace2Create>0)
			{
				vFaces[lcFaceLoop-1].vIndices.pop_back();
				--lcFace2Create;
			}

		}// end if nbVertices>3

		--lcFaceLoop;
	}

	for(int i=0; i<newNbrFace ; ++i)
	{
		vFaces.push_back(vNewFaces[i]);
	}
	NbrFace += newNbrFace;


}


void An8Mesh::ComputeFaceNormal(void)
{
	int lcFaceLoop;				// Face Loop
	float lcVx1,lcVy1,lcVz1;	// Temporary Vector
	float lcVx2,lcVy2,lcVz2;	// Temporary Vector
	float lcNx,lcNy,lcNz;		// Temporary Normal Vector
	float magnitude;			// To Normalize The Normal

	lcFaceLoop = NbrFace;

	while(lcFaceLoop>0) // For all faces
	{
		// Compute two vectors of the face
		lcVx1 = vPoints[ vFaces[lcFaceLoop-1].vIndices[0].Point ].x - vPoints[ vFaces[lcFaceLoop-1].vIndices[1].Point ].x;
		lcVy1 = vPoints[ vFaces[lcFaceLoop-1].vIndices[0].Point ].y - vPoints[ vFaces[lcFaceLoop-1].vIndices[1].Point ].y;
		lcVz1 = vPoints[ vFaces[lcFaceLoop-1].vIndices[0].Point ].z - vPoints[ vFaces[lcFaceLoop-1].vIndices[1].Point ].z;

		lcVx2 = vPoints[vFaces[lcFaceLoop-1].vIndices[1].Point].x - vPoints[vFaces[lcFaceLoop-1].vIndices[2].Point].x;
		lcVy2 = vPoints[vFaces[lcFaceLoop-1].vIndices[1].Point].y - vPoints[vFaces[lcFaceLoop-1].vIndices[2].Point].y;
		lcVz2 = vPoints[vFaces[lcFaceLoop-1].vIndices[1].Point].z - vPoints[vFaces[lcFaceLoop-1].vIndices[2].Point].z;

		// Compute normals
		lcNx=( (lcVy2*lcVz1) - (lcVy1*lcVz2) );
		lcNy=( (lcVx1*lcVz2) - (lcVx2*lcVz1) );
		lcNz=( (lcVx2*lcVy1) - (lcVx1*lcVy2) );

		magnitude=(float) sqrt( (lcNx*lcNx)
							+ (lcNy*lcNy)
							+ (lcNz*lcNz)
		);

        if( magnitude < 0.000001 && magnitude > -0.000001 )
        { // The face is just a point
			// Default normal
			vFaces[lcFaceLoop-1].Normal.x = 0.0f;
			vFaces[lcFaceLoop-1].Normal.y = 1.0f;
			vFaces[lcFaceLoop-1].Normal.z = 0.0f;
        }
		else
		{
			// Normalize the normals
			vFaces[lcFaceLoop-1].Normal.x = lcNx/magnitude;
			vFaces[lcFaceLoop-1].Normal.y = lcNy/magnitude;
			vFaces[lcFaceLoop-1].Normal.z = lcNz/magnitude;
		}

	--lcFaceLoop;
	}
}


void An8Mesh::ComputeBoundingElements(void)
{
	int i_point  = 0;

	float lcNewX,lcNewY,lcNewZ;
	lcNewX=lcNewY=lcNewZ=0.0f;

	float minX,maxX,minY,maxY,minZ,maxZ;
	minX=maxX=minY=maxY=minZ=maxZ=0.0f;

	minX = minY = minZ =  9999999.0f;
	maxX = maxY = maxZ = -9999999.0f;

	for(i_point = 0 ; i_point < NbrPoint ; ++i_point)
	{

		lcNewX = vPoints[i_point].x;
		lcNewY = vPoints[i_point].y;
		lcNewZ = vPoints[i_point].z;

        if(lcNewX<minX)
          minX=lcNewX;
        if(lcNewX>maxX)
          maxX=lcNewX;

        if(lcNewY<minY)
          minY=lcNewY;
        if(lcNewY>maxY)
          maxY=lcNewY;

        if(lcNewZ<minZ)
          minZ=lcNewZ;
        if(lcNewZ>maxZ)
          maxZ=lcNewZ;

	} // End of point

	BoundingBox.Min.x = minX;
	BoundingBox.Max.x = maxX;

	BoundingBox.Min.y = minY;
	BoundingBox.Max.y = maxY;

	BoundingBox.Min.z = minZ;
	BoundingBox.Max.z = maxZ;

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

//////////////////////////////////////////////////////////
//	Compute Mesh Matrix								//
//////////////////////////////////////////////////////////
void An8Mesh::ComputeMatrix(bool includeTranslation)
{
	AN8XMATRIX matrixTmp;
	AN8XMatrixRotationQuaternion( &matrixTmp , &BaseOrientation );
	if( includeTranslation == true )
	{
		matrixTmp[12] = BaseOrigin.x;
		matrixTmp[13] = BaseOrigin.y;
		matrixTmp[14] = BaseOrigin.z;
	}
	/*if( isGL == true )
	{*/
		AN8XMatrixCopy( &Matrix , &matrixTmp);
	/*}
	else
	{
		AN8XMatrixTranspose( &Matrix , &matrixTmp);
	}*/
}




//////////////////////////////////////////////////////////
//	Compute mesh position								//
//////////////////////////////////////////////////////////
void An8Mesh::ComputePosition(AN8XQUATERNION *i_pBaseOrientation,AN8XVECTOR3* i_pBaseOrigin)
{
	if(i_pBaseOrientation != NULL && i_pBaseOrigin != NULL)
	{
		/*
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

	float Matrix[3][3];
	float newX,newY,newZ;

	int lcPointLoop;

	// Quaternion -> matrix
	Matrix[0][0]= 1-(2*BaseOrientation.y*BaseOrientation.y)
						-(2*BaseOrientation.z*BaseOrientation.z);
	Matrix[1][0]= (2*BaseOrientation.x*BaseOrientation.y)
						-(2*BaseOrientation.w*BaseOrientation.z);
	Matrix[2][0]= (2*BaseOrientation.x*BaseOrientation.z)
						+(2*BaseOrientation.w*BaseOrientation.y);

	Matrix[0][1]= (2*BaseOrientation.x*BaseOrientation.y)
						+(2*BaseOrientation.w*BaseOrientation.z);
	Matrix[1][1]= 1-(2*BaseOrientation.x*BaseOrientation.x)
						-(2*BaseOrientation.z*BaseOrientation.z);
	Matrix[2][1]= (2*BaseOrientation.y*BaseOrientation.z)
						-(2*BaseOrientation.w*BaseOrientation.x);

	Matrix[0][2]= (2*BaseOrientation.x*BaseOrientation.z)
						-(2*BaseOrientation.w*BaseOrientation.y);
	Matrix[1][2]= (2*BaseOrientation.y*BaseOrientation.z)
						+(2*BaseOrientation.w*BaseOrientation.x);
	Matrix[2][2]= 1-(2*BaseOrientation.x*BaseOrientation.x)
						-(2*BaseOrientation.y*BaseOrientation.y);

	/// Points ////////////////
	lcPointLoop=NbrPoint;

	while(lcPointLoop>0) // for all Points
	{
		// compute new orientation
		newX=Matrix[0][0]*vPoints[lcPointLoop-1].x
					+Matrix[1][0]*vPoints[lcPointLoop-1].y
					+Matrix[2][0]*vPoints[lcPointLoop-1].z;
		newY=Matrix[0][1]*vPoints[lcPointLoop-1].x
					+Matrix[1][1]*vPoints[lcPointLoop-1].y
					+Matrix[2][1]*vPoints[lcPointLoop-1].z;
		newZ=Matrix[0][2]*vPoints[lcPointLoop-1].x
					+Matrix[1][2]*vPoints[lcPointLoop-1].y
					+Matrix[2][2]*vPoints[lcPointLoop-1].z;

		// update new position
		vPoints[lcPointLoop-1].x = BaseOrigin.x + newX;
		vPoints[lcPointLoop-1].y = BaseOrigin.y + newY;
		vPoints[lcPointLoop-1].z = BaseOrigin.z + newZ;

		--lcPointLoop;
	}

	/// Normals ////////////////
	lcPointLoop=NbrNormal;

	while(lcPointLoop>0) // for all Normals
	{
		// compute new orientation
		newX=Matrix[0][0]*vNormals[lcPointLoop-1].x
					+Matrix[1][0]*vNormals[lcPointLoop-1].y
					+Matrix[2][0]*vNormals[lcPointLoop-1].z;
		newY=Matrix[0][1]*vNormals[lcPointLoop-1].x
					+Matrix[1][1]*vNormals[lcPointLoop-1].y
					+Matrix[2][1]*vNormals[lcPointLoop-1].z;
		newZ=Matrix[0][2]*vNormals[lcPointLoop-1].x
					+Matrix[1][2]*vNormals[lcPointLoop-1].y
					+Matrix[2][2]*vNormals[lcPointLoop-1].z;

		// update new position
		vNormals[lcPointLoop-1].x = newX;
		vNormals[lcPointLoop-1].y = newY;
		vNormals[lcPointLoop-1].z = newZ;

		--lcPointLoop;
	}

	BaseOrientation.x = 0.0f;
	BaseOrientation.y = 0.0f;
	BaseOrientation.z = 0.0f;
	BaseOrientation.w = 1.0f;

	BaseOrigin.x = 0.0f;
	BaseOrigin.y = 0.0f;
	BaseOrigin.z = 0.0f;
}



void An8Mesh::ComputeTangentsBinormals(void)
{
	for( int i = 0 ; i < NbrFace ; ++i )
	{
		// Variation among XYZ
		AN8XVECTOR3 vect_01 = AN8XVECTOR3(
			vPoints[vFaces[i].vIndices[1].Point].x - vPoints[vFaces[i].vIndices[0].Point].x ,
			vPoints[vFaces[i].vIndices[1].Point].y - vPoints[vFaces[i].vIndices[0].Point].y ,
			vPoints[vFaces[i].vIndices[1].Point].z - vPoints[vFaces[i].vIndices[0].Point].z
		);
		AN8XVECTOR3 vect_21 = AN8XVECTOR3(
			vPoints[vFaces[i].vIndices[2].Point].x - vPoints[vFaces[i].vIndices[1].Point].x ,
			vPoints[vFaces[i].vIndices[2].Point].y - vPoints[vFaces[i].vIndices[1].Point].y ,
			vPoints[vFaces[i].vIndices[2].Point].z - vPoints[vFaces[i].vIndices[1].Point].z
		);

		// Variation among the UV
		float delatS_01 = vTexCoords[vFaces[i].vIndices[1].TexCoord].x - vTexCoords[vFaces[i].vIndices[0].TexCoord].x;
		float delatS_21 = vTexCoords[vFaces[i].vIndices[2].TexCoord].x - vTexCoords[vFaces[i].vIndices[1].TexCoord].x;

		float delatT_01 = vTexCoords[vFaces[i].vIndices[1].TexCoord].y - vTexCoords[vFaces[i].vIndices[0].TexCoord].y;
		float delatT_21 = vTexCoords[vFaces[i].vIndices[2].TexCoord].y - vTexCoords[vFaces[i].vIndices[1].TexCoord].y;

		AN8XVECTOR3 sTangent1 = vect_01*delatT_21 - vect_21*delatT_01;
		AN8XVECTOR3 binormal1 = vect_01*(-delatS_21) + vect_21*delatS_01;

		vFaces[i].vertexBinormals[1] = AN8XVECTOR3(sTangent1.x,sTangent1.y,sTangent1.z);
		vFaces[i].vertexTangents[1]  = AN8XVECTOR3(binormal1.x,binormal1.y,binormal1.z);

		//An8_Normalize(&vFaces[i].vertexBinormals[1]);
		//An8_Normalize(&vFaces[i].vertexTangents[1]);
		AN8XVec3Normalize(&vFaces[i].vertexBinormals[1],&vFaces[i].vertexBinormals[1]);
		AN8XVec3Normalize(&vFaces[i].vertexTangents[1],&vFaces[i].vertexTangents[1]);

		//////////////////////////////////////////////


		// Variation among XYZ
		AN8XVECTOR3 vect_12 = AN8XVECTOR3(
			vPoints[vFaces[i].vIndices[2].Point].x - vPoints[vFaces[i].vIndices[1].Point].x ,
			vPoints[vFaces[i].vIndices[2].Point].y - vPoints[vFaces[i].vIndices[1].Point].y ,
			vPoints[vFaces[i].vIndices[2].Point].z - vPoints[vFaces[i].vIndices[1].Point].z
		);
		AN8XVECTOR3 vect_02 = AN8XVECTOR3(
			vPoints[vFaces[i].vIndices[0].Point].x - vPoints[vFaces[i].vIndices[2].Point].x ,
			vPoints[vFaces[i].vIndices[0].Point].y - vPoints[vFaces[i].vIndices[2].Point].y ,
			vPoints[vFaces[i].vIndices[0].Point].z - vPoints[vFaces[i].vIndices[2].Point].z
		);

		// Variation among the UV
		float delatS_12 = vTexCoords[vFaces[i].vIndices[2].TexCoord].x - vTexCoords[vFaces[i].vIndices[1].TexCoord].x;
		float delatS_02 = vTexCoords[vFaces[i].vIndices[0].TexCoord].x - vTexCoords[vFaces[i].vIndices[2].TexCoord].x;

		float delatT_12 = vTexCoords[vFaces[i].vIndices[2].TexCoord].y - vTexCoords[vFaces[i].vIndices[1].TexCoord].y;
		float delatT_02 = vTexCoords[vFaces[i].vIndices[0].TexCoord].y - vTexCoords[vFaces[i].vIndices[2].TexCoord].y;


		AN8XVECTOR3 sTangent2 = vect_12*delatT_02 - vect_02*delatT_12;
		AN8XVECTOR3 binormal2 = vect_12*(-delatS_02) + vect_02*delatS_12;

		vFaces[i].vertexBinormals[2] = AN8XVECTOR3(sTangent2.x,sTangent2.y,sTangent2.z);
		vFaces[i].vertexTangents[2] = AN8XVECTOR3(binormal2.x,binormal2.y,binormal2.z);
		//An8_Normalize(&vFaces[i].vertexBinormals[2]);
		//An8_Normalize(&vFaces[i].vertexTangents[2]);
		AN8XVec3Normalize(&vFaces[i].vertexBinormals[2],&vFaces[i].vertexBinormals[2]);
		AN8XVec3Normalize(&vFaces[i].vertexTangents[2],&vFaces[i].vertexTangents[2]);


		// Variation among XYZ
		AN8XVECTOR3 vect_20 = AN8XVECTOR3(
			vPoints[0].x - vPoints[2].x ,
			vPoints[0].y - vPoints[2].y ,
			vPoints[0].z - vPoints[2].z
		);
		AN8XVECTOR3 vect_10 = AN8XVECTOR3(
			vPoints[1].x - vPoints[0].x ,
			vPoints[1].y - vPoints[0].y ,
			vPoints[1].z - vPoints[0].z
		);

		// Variation among the UV
		float delatS_20 = vTexCoords[vFaces[i].vIndices[0].TexCoord].x - vTexCoords[vFaces[i].vIndices[2].TexCoord].x;
		float delatS_10 = vTexCoords[vFaces[i].vIndices[1].TexCoord].x - vTexCoords[vFaces[i].vIndices[0].TexCoord].x;

		float delatT_20 = vTexCoords[vFaces[i].vIndices[0].TexCoord].y - vTexCoords[vFaces[i].vIndices[2].TexCoord].y;
		float delatT_10 = vTexCoords[vFaces[i].vIndices[1].TexCoord].y - vTexCoords[vFaces[i].vIndices[0].TexCoord].y;

		AN8XVECTOR3 sTangent3 = vect_20*delatT_10 - vect_10*delatT_20;
		AN8XVECTOR3 binormal3 = vect_20*(-delatS_10) + vect_10*delatS_20;

		vFaces[i].vertexBinormals[0] = AN8XVECTOR3(sTangent3.x,sTangent3.y,sTangent3.z);
		vFaces[i].vertexTangents[0] = AN8XVECTOR3(binormal3.x,binormal3.y,binormal3.z);
		//An8_Normalize(&vFaces[i].vertexBinormals[0]);
		//An8_Normalize(&vFaces[i].vertexTangents[0]);
		AN8XVec3Normalize(&vFaces[i].vertexBinormals[0],&vFaces[i].vertexBinormals[0]);
		AN8XVec3Normalize(&vFaces[i].vertexTangents[0],&vFaces[i].vertexTangents[0]);

		} // While Face
}




void An8Mesh::ComputeSmoothNormals(void)
{
	int nbrPoint = NbrPoint;
	for( int p = 0 ; p < nbrPoint ; ++p )
	{
		AN8XVECTOR3 normal(0.0f,0.0f,0.0f);
		for( int f = 0 ; f < NbrFace ; ++f)
		{
			float angle;
			if( vFaces[f].vIndices[0].Point == p )
			{
				AN8XVECTOR3 vec0 = vPoints[vFaces[f].vIndices[1].Point] - vPoints[p];
				AN8XVECTOR3 vec1 = vPoints[vFaces[f].vIndices[2].Point] - vPoints[p];
				angle =  1.0f;//AN8XAngleBetweenVec3(&vec0,&vec1);
				normal = normal +(vFaces[f].Normal * angle);
			}
			else if( vFaces[f].vIndices[1].Point == p )
			{
				AN8XVECTOR3 vec0 = vPoints[vFaces[f].vIndices[2].Point] - vPoints[p];
				AN8XVECTOR3 vec1 = vPoints[vFaces[f].vIndices[0].Point] - vPoints[p];
				angle =  1.0f;//AN8XAngleBetweenVec3(&vec0,&vec1);
				normal = normal + (vFaces[f].Normal * angle);
			}
			else if( vFaces[f].vIndices[2].Point == p )
			{
				AN8XVECTOR3 vec0 = vPoints[vFaces[f].vIndices[0].Point] - vPoints[p];
				AN8XVECTOR3 vec1 = vPoints[vFaces[f].vIndices[1].Point] - vPoints[p];
				angle =  1.0f;//AN8XAngleBetweenVec3(&vec0,&vec1);
				normal = normal + (vFaces[f].Normal * angle);
			}
		} // End of faces
		AN8XVec3Normalize(&normal,&normal);
		vSmoothNormals.push_back(normal);
	} // End of point
}

/*
void An8Mesh::ComputeSmoothNormals(void)
{
	int nbrPoint = NbrPoint;
	for( int p = 0 ; p < nbrPoint ; ++p )
	{
		//float totalAngle = 0.0f;
		AN8XVECTOR3 normal(0.0f,0.0f,0.0f);
		for( int f = 0 ; f < NbrFace ; ++f)
		{
			float angle;
			if( vFaces[f].vIndices[0].Point == p )
			{
				AN8XVECTOR3 vec0 = vPoints[vFaces[f].vIndices[1].Point] - vPoints[vFaces[f].vIndices[0].Point];
				AN8XVECTOR3 vec1 = vPoints[vFaces[f].vIndices[2].Point] - vPoints[vFaces[f].vIndices[0].Point];
				angle =  AN8XAngleBetweenVec3(&vec0,&vec1);
				normal = normal +(vNormals[vFaces[f].vIndices[0].Normal] * angle);
			}
			else if( vFaces[f].vIndices[1].Point == p )
			{
				AN8XVECTOR3 vec0 = vPoints[vFaces[f].vIndices[0].Point] - vPoints[vFaces[f].vIndices[1].Point];
				AN8XVECTOR3 vec1 = vPoints[vFaces[f].vIndices[2].Point] - vPoints[vFaces[f].vIndices[1].Point];
				angle =  AN8XAngleBetweenVec3(&vec0,&vec1);
				normal = normal + vNormals[vFaces[f].vIndices[1].Normal] * angle;
			}
			else if( vFaces[f].vIndices[1].Point == p )
			{
				AN8XVECTOR3 vec0 = vPoints[vFaces[f].vIndices[1].Point] - vPoints[vFaces[f].vIndices[2].Point];
				AN8XVECTOR3 vec1 = vPoints[vFaces[f].vIndices[0].Point] - vPoints[vFaces[f].vIndices[2].Point];
				angle =  AN8XAngleBetweenVec3(&vec0,&vec1);
				normal = normal + vNormals[vFaces[f].vIndices[2].Normal] * angle;
			}
			int toto = 3;
		} // End of faces

		AN8XVec3Normalize(&normal,&normal);
		vSmoothNormals.push_back(normal);
	} // End of point
}
*/
