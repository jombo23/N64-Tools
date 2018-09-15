// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// Web Site: texel3d.free.fr

#include"An8Loader.h"
#include<math.h>

An8Mesh* AN8XFacesOfCube( An8Mesh* i_Mesh, An8Cube* i_Cube, 
						 int* i_Top, int* i_Left,int* i_Bottom ,int* i_Right, 
						 int i_DivX, int i_DivY, int i_NormalIndex, int i_TextureOffset, int i_Option )
{
	int** faces;
	An8FaceIndice index;

	int faceMask = 4;
	if( i_Option == AN8_NORMAL )
	{
		faceMask = 6;
	}


	faces = new int*[i_DivX+1];
	for( int i = 0 ; i < i_DivX+1 ; ++i )
	{
		faces[i] = new int[i_DivY+1];
	}

	// Border //////////////////////////////

	if( i_NormalIndex != 5)
	{
		for(int i = 0 ; i < i_DivY+1 ; ++i )
		{
			faces[0][i] = i_Left[i];
		}
	}
	else 
	{
		for(int i = 0 ; i < i_DivY+1 ; ++i )
		{
			faces[0][i] = i_Left[i_DivY - i];
		}
	}
	

	if( i_NormalIndex < 4 || i_NormalIndex == 5 )
	{
		for(int i = 0 ; i < i_DivY+1 ; ++i )
		{
			faces[i_DivX][i] = i_Right[i];
		}
		for(int i = 0 ; i < i_DivX+1 ; ++i )
		{
			faces[i][0] = i_Top[i];
		}
	}
	else
	{
		for(int i = 0 ; i < i_DivY+1 ; ++i )
		{
			faces[i_DivX][i] = i_Right[i_DivY-i];
		}
		for(int i = 0 ; i < i_DivX+1 ; ++i )
		{
			faces[i][0] = i_Top[i_DivX-i];
		}
	}

	if( i_NormalIndex != 5)
	{
		for(int i = 0 ; i < i_DivX+1 ; ++i )
		{
			faces[i][i_DivY] = i_Bottom[i];
		}
	}
	else
	{
		for(int i = 0 ; i < i_DivX+1 ; ++i )
		{
			faces[i][i_DivY] = i_Bottom[i_DivX-i];
		}
	}



	// New points

	for(int i = 1 ; i < i_DivX ; ++i )
	{
		AN8XVECTOR3 vec = i_Mesh->vPoints[ faces[i][i_DivY] ] - i_Mesh->vPoints[ faces[i][0] ];
		float length = AN8XVec3Length(&vec);
		AN8XVec3Normalize(&vec,&vec);

		for(int j = 1 ; j < i_DivY ; ++j )
		{
			AN8XVECTOR3 newPoint = i_Mesh->vPoints[ faces[i][0] ] + (vec*(float)j*length/(float)i_DivY);
			i_Mesh->vPoints.push_back(newPoint);
			faces[i][j] = (int)i_Mesh->vPoints.size()-1;
			
		}
	}

	// Create Face
	for( int x = 0 ; x < i_DivX; ++x )
	{
		for( int y = 0 ; y < i_DivY; ++y )
		{
			int v = i_Cube->DivX - 1 - x;
			An8Face newFace;
			newFace.NbrVertex = 4;
			newFace.Mask      = faceMask;
			newFace.Material  = 0;

			// 0	
			index.Point    = faces[x+1][y];
			index.Normal   = i_NormalIndex;
			if( i_NormalIndex == 0 )
				index.TexCoord = y + (x+1)*(i_Cube->DivY+1);
			if( i_NormalIndex == 2 )
				index.TexCoord = y + (v)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 4 || i_NormalIndex == 5 )
				index.TexCoord = (x+1)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 1 || i_NormalIndex == 3 )
				index.TexCoord = y + (i_Cube->DivX)*(i_Cube->DivY+1);
			newFace.vIndices.push_back(index);

			// 1 
			index.Point    = faces[x+1][y+1];
			index.Normal   = i_NormalIndex;
			if( i_NormalIndex == 0)
				index.TexCoord = (y+1) + (x+1)*(i_Cube->DivY+1);
			if( i_NormalIndex == 2 )
				index.TexCoord = (y+1) + (v)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 4 || i_NormalIndex == 5 )
				index.TexCoord = (x+1)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 1 || i_NormalIndex == 3 )
				index.TexCoord = y + 1 + (i_Cube->DivX)*(i_Cube->DivY+1);
			newFace.vIndices.push_back(index);

			// 2
			index.Point = faces[x][y+1];
			index.Normal = i_NormalIndex;
			if( i_NormalIndex == 0)
				index.TexCoord = (y+1) + (x)*(i_Cube->DivY+1);
			if( i_NormalIndex == 2 )
				index.TexCoord = (y+1) + (v+1)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 4 || i_NormalIndex == 5 )
				index.TexCoord = (x)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 1 || i_NormalIndex == 3 )
				index.TexCoord = y + 1 + (i_Cube->DivX)*(i_Cube->DivY+1);
			newFace.vIndices.push_back(index);

			// 3
			index.Point = faces[x][y];
			index.Normal = i_NormalIndex;
			if( i_NormalIndex == 0)
				index.TexCoord = (y) + (x)*(i_Cube->DivY+1);
			if( i_NormalIndex == 2 )
				index.TexCoord = (y) + (v+1)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 4 || i_NormalIndex == 5 )
				index.TexCoord = (x)*(i_Cube->DivY+1);
			else if ( i_NormalIndex == 1 || i_NormalIndex == 3 )
				index.TexCoord = y + (i_Cube->DivX)*(i_Cube->DivY+1);
			newFace.vIndices.push_back(index);

			i_Mesh->vFaces.push_back(newFace);
			++i_Mesh->NbrFace;
		}
	}


	// Delete /////////////////////////////

	for( int i = 0 ; i < i_DivX+1 ; ++i )
	{
		delete [] faces[i];
	}
	delete [] faces;

	return i_Mesh;
}


// ------------------------------------------------------
// Create a mesh from a cube (not triangulized)
// ------------------------------------------------------
An8Mesh* AN8XMeshFromCube( An8Mesh* i_Mesh, An8Cube* i_Cube, int i_Option  )
{
	if( i_Mesh != NULL || i_Cube == NULL )
		return NULL;

	int faceMask = 4;
	if( i_Option == AN8_NORMAL )
	{
		faceMask = 6;
	}

	i_Mesh = new An8Mesh();

	i_Mesh->Name = i_Cube->Name;
	i_Mesh->BaseOrigin = i_Cube->BaseOrigin; // 0 0 0 
	i_Mesh->BaseOrientation = i_Cube->BaseOrientation; // 0 0 0 

	i_Mesh->NbrMateriallist = 1;
	i_Mesh->vMatNumber.push_back(i_Cube->MaterialID);

	int* VertexIndicesVertical[4];
	VertexIndicesVertical[0] = new int[i_Cube->DivY+1];
	VertexIndicesVertical[1] = new int[i_Cube->DivY+1];
	VertexIndicesVertical[2] = new int[i_Cube->DivY+1];
	VertexIndicesVertical[3] = new int[i_Cube->DivY+1];

	AN8XVECTOR3 pointRef;AN8XVECTOR3 newPoint;
	float step = i_Cube->Scale.y / i_Cube->DivY;
		
	// Front Left
	pointRef.x = -(i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z =  (i_Cube->Scale.z / 2.0f);
	for( int i = 0 ; i < i_Cube->DivY+1 ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(0.0f,-step*i,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesVertical[0][i] = (int)i_Mesh->vPoints.size() - 1;
	}

	// Front Right
	pointRef.x =  (i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z =  (i_Cube->Scale.z / 2.0f);
	for( int i = 0 ; i < i_Cube->DivY+1 ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(0.0f,-step*i,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesVertical[1][i] = (int)i_Mesh->vPoints.size() - 1;
	}
	// Back Right
	pointRef.x =  (i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z = -(i_Cube->Scale.z / 2.0f);
	for( int i = 0 ; i < i_Cube->DivY+1 ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(0.0f,-step*i,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesVertical[2][i] = (int)i_Mesh->vPoints.size() - 1;
	}
	// Back Left
	pointRef.x = -(i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z = -(i_Cube->Scale.z / 2.0f);
	for( int i = 0 ; i < i_Cube->DivY+1 ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(0.0f,-step*i,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesVertical[3][i] = (int)i_Mesh->vPoints.size() - 1;
	}

	// Top
	int* VertexIndicesTop[4];
	VertexIndicesTop[0] = new int[i_Cube->DivX+1]; // Front
	VertexIndicesTop[1] = new int[i_Cube->DivZ+1]; // Right
	VertexIndicesTop[2] = new int[i_Cube->DivX+1]; // Back
	VertexIndicesTop[3] = new int[i_Cube->DivZ+1]; // Left

	// Bottom
	int* VertexIndicesBottom[4];
	VertexIndicesBottom[0] = new int[i_Cube->DivX+1]; // Front
	VertexIndicesBottom[1] = new int[i_Cube->DivZ+1]; // Right
	VertexIndicesBottom[2] = new int[i_Cube->DivX+1]; // Back
	VertexIndicesBottom[3] = new int[i_Cube->DivZ+1]; // Left

	step = i_Cube->Scale.x / i_Cube->DivX;

	// Top/Bottom Front
	pointRef.x = -(i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z =  (i_Cube->Scale.z / 2.0f);
	for( int i = 1 ; i < i_Cube->DivX ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(step*i,0.0f,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesTop[0][i] = (int)i_Mesh->vPoints.size() - 1;

		newPoint = newPoint + AN8XVECTOR3(0.0f,-i_Cube->Scale.y ,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesBottom[0][i] = (int)i_Mesh->vPoints.size() - 1;
	}
	VertexIndicesTop[0][0]    = VertexIndicesVertical[0][0];
	VertexIndicesBottom[0][0] = VertexIndicesVertical[0][i_Cube->DivY];

	VertexIndicesTop[0][i_Cube->DivX]    = VertexIndicesVertical[1][0];
	VertexIndicesBottom[0][i_Cube->DivX] = VertexIndicesVertical[1][i_Cube->DivY];

	// Top/Bottom Back
	pointRef.x =  -(i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z =  -(i_Cube->Scale.z / 2.0f);
	for( int i = 1 ; i < i_Cube->DivX ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(step*i,0.0f,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesTop[2][i_Cube->DivX-i] = (int)i_Mesh->vPoints.size() - 1;

		newPoint = newPoint + AN8XVECTOR3(0.0f,-i_Cube->Scale.y ,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesBottom[2][i_Cube->DivX-i] = (int)i_Mesh->vPoints.size() - 1;
	}
	VertexIndicesTop[2][0]    = VertexIndicesVertical[2][0];
	VertexIndicesBottom[2][0] = VertexIndicesVertical[2][i_Cube->DivY];

	VertexIndicesTop[2][i_Cube->DivX]    = VertexIndicesVertical[3][0];
	VertexIndicesBottom[2][i_Cube->DivX] = VertexIndicesVertical[3][i_Cube->DivY];

	step = i_Cube->Scale.z / i_Cube->DivZ;

	// Top/Bottom Right
	pointRef.x =  (i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z =  (i_Cube->Scale.z / 2.0f);
	for( int i = 1 ; i < i_Cube->DivZ ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(0.0f,0.0f,-step*i);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesTop[1][i] = (int)i_Mesh->vPoints.size() - 1;

		newPoint = newPoint + AN8XVECTOR3(0.0f,-i_Cube->Scale.y ,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesBottom[1][i] = (int)i_Mesh->vPoints.size() - 1;
	}
	VertexIndicesTop[1][0]    = VertexIndicesVertical[1][0];
	VertexIndicesBottom[1][0] = VertexIndicesVertical[1][i_Cube->DivY];

	VertexIndicesTop[1][i_Cube->DivX]    = VertexIndicesVertical[2][0];
	VertexIndicesBottom[1][i_Cube->DivX] = VertexIndicesVertical[2][i_Cube->DivY];

	// Top/Bottom Left
	pointRef.x = -(i_Cube->Scale.x / 2.0f);
	pointRef.y =  (i_Cube->Scale.y / 2.0f);
	pointRef.z =  (i_Cube->Scale.z / 2.0f);
	for( int i = 1 ; i < i_Cube->DivZ ; ++i )
	{
		newPoint = pointRef + AN8XVECTOR3(0.0f,0.0f,-step*i);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesTop[3][i_Cube->DivZ-i] = (int)i_Mesh->vPoints.size() - 1;

		newPoint = newPoint + AN8XVECTOR3(0.0f,-i_Cube->Scale.y ,0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		VertexIndicesBottom[3][i_Cube->DivZ-i] = (int)i_Mesh->vPoints.size() - 1;
	}
	VertexIndicesTop[3][0]    = VertexIndicesVertical[3][0];
	VertexIndicesBottom[3][0] = VertexIndicesVertical[3][i_Cube->DivY];

	VertexIndicesTop[3][i_Cube->DivX]    = VertexIndicesVertical[0][0];
	VertexIndicesBottom[3][i_Cube->DivX] = VertexIndicesVertical[0][i_Cube->DivY];


	// Texcoord Front/back
	for( int x = 0 ; x < i_Cube->DivX+1; ++x)
	{
		for( int y = 0 ; y < i_Cube->DivY+1; ++y)
		{
			AN8XVECTOR2 newTex( 
				(1.0f/i_Cube->DivX)*(float)x,
				1.0f-(1.0f/i_Cube->DivY)*(float)y
			);
			i_Mesh->vTexCoords.push_back(newTex);
		}
	}
	
	i_Mesh->NbrUVCoord = (int)i_Mesh->vTexCoords.size();


	AN8XFacesOfCube(
		i_Mesh,i_Cube,
		VertexIndicesTop[0],VertexIndicesVertical[0],VertexIndicesBottom[0],VertexIndicesVertical[1],
		i_Cube->DivX,i_Cube->DivY,
		0,
		0, // Texture Offset
		i_Option
	);
	
	AN8XFacesOfCube(
		i_Mesh,i_Cube,
		VertexIndicesTop[1],VertexIndicesVertical[1],VertexIndicesBottom[1],VertexIndicesVertical[2],
		i_Cube->DivZ,i_Cube->DivY,
		1,
		i_Cube->DivX*i_Cube->DivY, // Texture Offset
		i_Option
	);
	
	AN8XFacesOfCube(
		i_Mesh,i_Cube,
		VertexIndicesTop[2],VertexIndicesVertical[2],VertexIndicesBottom[2],VertexIndicesVertical[3],
		i_Cube->DivX,i_Cube->DivY,
		2,
		0, // Texture Offset
		i_Option
	);
	
	AN8XFacesOfCube(
		i_Mesh,i_Cube,
		VertexIndicesTop[3],VertexIndicesVertical[3],VertexIndicesBottom[3],VertexIndicesVertical[0],
		i_Cube->DivZ,i_Cube->DivY,
		3,
		i_Cube->DivX*i_Cube->DivY, // Texture Offset
		i_Option
	);

	AN8XFacesOfCube(
		i_Mesh,i_Cube,
		VertexIndicesTop[2],VertexIndicesTop[3],VertexIndicesTop[0],VertexIndicesTop[1],
		i_Cube->DivX,i_Cube->DivZ,
		4,
		(i_Cube->DivX*i_Cube->DivY)+(i_Cube->DivZ*i_Cube->DivY), // Texture Offset
		i_Option
	);

	AN8XFacesOfCube(
		i_Mesh,i_Cube,
		VertexIndicesBottom[2],VertexIndicesBottom[3],VertexIndicesBottom[0],VertexIndicesBottom[1],
		i_Cube->DivX,i_Cube->DivZ,
		5,
		(i_Cube->DivX*i_Cube->DivY)+(i_Cube->DivZ*i_Cube->DivY), // Texture Offset
		i_Option
	);

	if( i_Option == AN8_NORMAL )
	{
		AN8XVECTOR3 newPoint = AN8XVECTOR3(0.0f,0.0f,1.0f);
		i_Mesh->vNormals.push_back(newPoint);

		newPoint = AN8XVECTOR3(1.0f,0.0f,0.0f);
		i_Mesh->vNormals.push_back(newPoint);

		newPoint = AN8XVECTOR3(0.0f,0.0f,-1.0f);
		i_Mesh->vNormals.push_back(newPoint);

		newPoint = AN8XVECTOR3(-1.0f,0.0f,0.0f);
		i_Mesh->vNormals.push_back(newPoint);

		newPoint = AN8XVECTOR3(0.0f,1.0f,0.0f);
		i_Mesh->vNormals.push_back(newPoint);

		newPoint = AN8XVECTOR3(0.0f,-1.0f,0.0f);
		i_Mesh->vNormals.push_back(newPoint);

		i_Mesh->NbrNormal = 6;
	}




	for( int i = 0 ; i < 4 ; ++i )
	{
		delete [] VertexIndicesVertical[i];
		delete [] VertexIndicesTop[i];
		delete [] VertexIndicesBottom[i];
	}

	i_Mesh->NbrPoint = (int)i_Mesh->vPoints.size();
	return i_Mesh;
}

