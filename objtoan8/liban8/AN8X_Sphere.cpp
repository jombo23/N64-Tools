// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// Web Site: texel3d.free.fr

#include"An8Loader.h"
#include<math.h>

AN8XVECTOR2* AN8XSphericalUVFromNormal( AN8XVECTOR2* o_UV, AN8XVECTOR3* i_Normal  )
{

	//float perimeter = 2*AN8X_PI; // 2*Pi*R
	AN8XVECTOR3 ref = AN8XVECTOR3(1.0f,0.0f,0.0f);

	AN8XVECTOR3 vec3 = *i_Normal;
	vec3.y = 0.0f;
	float angle = AN8XAngleBetweenVec3(&ref,&vec3);

	if( i_Normal->z < 0.0f )
		o_UV->x = angle/(2*AN8X_PI);
	else
		o_UV->x = 1.0f - ( angle/(2*AN8X_PI) );

	o_UV->y = ( i_Normal->y / 2.0f ) + 0.5f;

	return o_UV;
}


An8Mesh* AN8XFacesOfGeodesicSphere( An8Mesh* i_Mesh, An8Sphere* i_Sphere,
								   int* i_Left, int* i_Right,int* i_Bottom ,
								   int* i_uvLeft, int* i_uvRight,int* i_uvBottom ,
								   bool i_FaceOrder, int i_Option )
{
	int level[7][7];
	int uvlevel[7][7];
	An8FaceIndice index0;
	An8FaceIndice index1;
	An8FaceIndice index2;

	int faceMask = 4;
	if( i_Option == AN8_NORMAL )
	{
		faceMask = 6;
	}

	// Create new vertices + copy indices in level
	for( int v = 0 ; v <= i_Sphere->Geodesic; ++v )
	{
		level[v][0] = i_Left[v];
		uvlevel[v][0] = i_uvLeft[v];
	}
	for( int v = 0 ; v <= i_Sphere->Geodesic; ++v )
	{
		level[v][v] = i_Right[v];
		uvlevel[v][v] = i_uvRight[v];
	}
	for( int h = 1 ; h < i_Sphere->Geodesic; ++h )
	{
		level  [i_Sphere->Geodesic][h] = i_Bottom[h];
		uvlevel[i_Sphere->Geodesic][h] = i_uvBottom[h];
	}
	for( int v = 2 ; v < i_Sphere->Geodesic; ++v )
	{
		AN8XVECTOR3 vec = i_Mesh->vPoints[level[v][v]] - i_Mesh->vPoints[level[v][0]];
		float length = AN8XVec3Length(&vec);
		AN8XVec3Normalize(&vec,&vec);

		AN8XVECTOR2 vecUV = i_Mesh->vTexCoords[uvlevel[v][v]] - i_Mesh->vTexCoords[uvlevel[v][0]];
		float lengthUV = AN8XVec2Length(&vecUV);
		AN8XVec2Normalize(&vecUV,&vecUV);

		for( int h = 1 ; h < v ; ++h )
		{
			AN8XVECTOR3 newPoint = i_Mesh->vPoints[level[v][0]] + (vec * (h*length/(float)v));
			i_Mesh->vPoints.push_back(newPoint);
			level[v][h] = (int)i_Mesh->vPoints.size()-1;

			// ToDo: Create uv
			AN8XVECTOR2 newUV = i_Mesh->vTexCoords[uvlevel[v][0]] + (vecUV * (h*lengthUV/(float)v));
			i_Mesh->vTexCoords.push_back(newUV);
			uvlevel[v][h] = (int)i_Mesh->vTexCoords.size() -1 ;



		}
	}


	// Create Face
	for( int v = 0 ; v < i_Sphere->Geodesic; ++v )
	{
		for( int i = 0 ; i < v+1; ++i )
		{
			An8Face newFace;
			newFace.NbrVertex = 3;
			newFace.Mask      = faceMask;
			newFace.Material  = 0;

			// 0
			index0.Point    = level[v][i];
			index0.Normal   = index0.Point;
			index0.TexCoord = uvlevel[v][i];

			// 1
			index1.Point = level[v+1][i];
			index1.Normal = index1.Point;
			index1.TexCoord = uvlevel[v+1][i];

			// 2
			index2.Point    = level[v+1][i+1];
			index2.Normal   = index2.Point;
			index2.TexCoord = uvlevel[v+1][i+1];

			if( i_FaceOrder == false )
			{
				newFace.vIndices.push_back(index0);
				newFace.vIndices.push_back(index1);
				newFace.vIndices.push_back(index2);
			}
			else
			{
				newFace.vIndices.push_back(index0);
				newFace.vIndices.push_back(index2);
				newFace.vIndices.push_back(index1);
			}

			i_Mesh->vFaces.push_back(newFace);
			++i_Mesh->NbrFace;
		}
		for( int i = 0 ; i < v ; ++i )
		{
			An8Face newFace;
			newFace.NbrVertex = 3;
			newFace.Mask = faceMask;
			newFace.Material = 0;

			// 0
			index0.Point = level[v][i];
			index0.Normal   = index0.Point;
			index0.TexCoord = uvlevel[v][i];

			// 1
			index1.Point    = level[v+1][i+1];
			index1.Normal   = index1.Point;
			index1.TexCoord = uvlevel[v+1][i+1];

			// 2
			index2.Point    = level[v][i+1];
			index2.Normal   = index2.Point;
			index2.TexCoord = uvlevel[v][i+1];

			if( i_FaceOrder == false )
			{
				newFace.vIndices.push_back(index0);
				newFace.vIndices.push_back(index1);
				newFace.vIndices.push_back(index2);
			}
			else
			{
				newFace.vIndices.push_back(index0);
				newFace.vIndices.push_back(index2);
				newFace.vIndices.push_back(index1);
			}

			i_Mesh->vFaces.push_back(newFace);
			++i_Mesh->NbrFace;
		}

	}




	return i_Mesh;
}

An8Mesh* AN8XMeshFromGeodesicSphere( An8Mesh* i_Mesh, An8Sphere* i_Sphere, int i_Option  )
{
	if( i_Mesh != NULL || i_Sphere == NULL || i_Sphere->Kind == 0 || i_Sphere->Geodesic > 6 )
		return NULL;


	i_Mesh = new An8Mesh();
	i_Mesh->Name = i_Sphere->Name;
	i_Mesh->BaseOrigin = i_Sphere->BaseOrigin; // 0 0 0
	i_Mesh->BaseOrientation = i_Sphere->BaseOrientation; // 0 0 0

	i_Mesh->NbrMateriallist = 1;
	i_Mesh->vMatNumber.push_back(i_Sphere->MaterialID);

	// Vertex indices
	int TopFront[7];
	int TopBack[7];
	int TopRight[7];
	int TopLeft[7];

	int BottomFront[7];
	int BottomBack[7];
	int BottomRight[7];
	int BottomLeft[7];

	int RightFront[7];
	int RightBack[7];
	int LeftFront[7];
	int LeftBack[7];

	// UV top
	int uvTopFront0[7];
	int uvTopRight0[7];
	int uvRightFront0[7];

	int uvTopRight1[7];
	int uvTopBack1[7];
	int uvRightBack1[7];

	int uvTopLeft2[7];
	int uvTopBack2[7];
	int uvLeftBack2[7];

	int uvTopLeft3[7];
	int uvTopFront3[7];
	int uvLeftFront3[7];

	// Uv bottom
	int uvBottomFront4[7];
	int uvBottomRight4[7];
	//int uvRightFront4[7];

	int uvBottomRight5[7];
	int uvBottomBack5[7];
	//int uvRightBack5[7];

	int uvBottomLeft6[7];
	int uvBottomBack6[7];
	//int uvLeftFront6[7];

	int uvBottomLeft7[7];
	int uvBottomFront7[7];
	//int uvLeftBack7[7];






	float radius = 1.0f;//i_Sphere->Diameter/2.0f;

	// Points of Level 0 ////////////////////////////////////////////

	// Point Top ////////////////////////////////////////////////////
	AN8XVECTOR3 newPoint = AN8XVECTOR3(0.0f,radius,0.0f);
	i_Mesh->vPoints.push_back(newPoint);
	++i_Mesh->NbrPoint;
	TopFront[0] = (int)i_Mesh->vPoints.size() -1 ;
	TopBack [0] = (int)i_Mesh->vPoints.size() -1 ;
	TopRight[0] = (int)i_Mesh->vPoints.size() -1 ;
	TopLeft [0] = (int)i_Mesh->vPoints.size() -1 ;

	AN8XVECTOR2 newUV = AN8XVECTOR2(0.875f,1.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopFront0[0] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvTopRight0[0] = (int)i_Mesh->vTexCoords.size() -1 ;

	newUV = AN8XVECTOR2(0.125f,1.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopRight1[0] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvTopBack1[0]  = (int)i_Mesh->vTexCoords.size() -1 ;

	newUV = AN8XVECTOR2(0.375f,1.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopLeft2[0] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvTopBack2[0] = (int)i_Mesh->vTexCoords.size() -1 ;

	newUV = AN8XVECTOR2(0.625f,1.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopLeft3[0]  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvTopFront3[0] = (int)i_Mesh->vTexCoords.size() -1 ;


	// Point Bottom /////////////////////////////////////////////////
	newPoint = AN8XVECTOR3(0.0f,-radius,0.0f);
	i_Mesh->vPoints.push_back(newPoint);
	++i_Mesh->NbrPoint;
	BottomFront[0] = (int)i_Mesh->vPoints.size() -1 ;
	BottomBack [0] = (int)i_Mesh->vPoints.size() -1 ;
	BottomRight[0] = (int)i_Mesh->vPoints.size() -1 ;
	BottomLeft [0] = (int)i_Mesh->vPoints.size() -1 ;

	newUV = AN8XVECTOR2(0.875f,0.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvBottomFront4[0] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomRight4[0] = (int)i_Mesh->vTexCoords.size() -1 ;

	newUV = AN8XVECTOR2(0.125f,0.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvBottomRight5[0] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomBack5[0]  = (int)i_Mesh->vTexCoords.size() -1 ;

	newUV = AN8XVECTOR2(0.375f,0.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvBottomLeft6[0] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomBack6[0] = (int)i_Mesh->vTexCoords.size() -1 ;

	newUV = AN8XVECTOR2(0.625f,0.0f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvBottomLeft7[0]  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomFront7[0] = (int)i_Mesh->vTexCoords.size() -1 ;


	// Point Back ///////////////////////////////////////////////////
	newPoint = AN8XVECTOR3(0.0f,0.0f,-radius);
	i_Mesh->vPoints.push_back(newPoint);
	++i_Mesh->NbrPoint;
	TopBack [i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	RightBack[i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	LeftBack [0] = (int)i_Mesh->vPoints.size() -1 ;
	BottomBack [i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;

	newUV = AN8XVECTOR2(0.25f,0.5f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopBack1 [i_Sphere->Geodesic]    = (int)i_Mesh->vTexCoords.size() -1 ;
	uvTopBack2 [i_Sphere->Geodesic]    = (int)i_Mesh->vTexCoords.size() -1 ;

	uvBottomBack5 [i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomBack6 [i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;

	uvRightBack1[i_Sphere->Geodesic]   = (int)i_Mesh->vTexCoords.size() -1 ;
	uvLeftBack2 [0]					  = (int)i_Mesh->vTexCoords.size() -1 ;




	// Point Front ///////////////////////////////////////////////////
	newPoint = AN8XVECTOR3(0.0f,0.0f,radius);
	i_Mesh->vPoints.push_back(newPoint);
	++i_Mesh->NbrPoint;
	TopFront[i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	BottomFront[i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	LeftFront[i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	RightFront[0]                 = (int)i_Mesh->vPoints.size() -1 ;

	newUV = AN8XVECTOR2(0.75f,0.5f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopFront0 [i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvTopFront3 [i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;

	uvBottomFront4 [i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomFront7 [i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;

	uvRightFront0[0]                  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvLeftFront3 [i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;






	// Point Right ///////////////////////////////////////////////////
	newPoint = AN8XVECTOR3(radius,0.0f,0.0f);
	i_Mesh->vPoints.push_back(newPoint);
	++i_Mesh->NbrPoint;
	TopRight[i_Sphere->Geodesic]   = (int)i_Mesh->vPoints.size() -1 ;
	BottomRight[i_Sphere->Geodesic]   = (int)i_Mesh->vPoints.size() -1 ;
	RightBack[0]                   = (int)i_Mesh->vPoints.size() -1 ;
	RightFront[i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;

	newUV = AN8XVECTOR2(0.0f,0.5f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopRight1[i_Sphere->Geodesic]  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvRightBack1[0]                  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomRight5[i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;

	newUV = AN8XVECTOR2(1.0f,0.5f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopRight0[i_Sphere->Geodesic]    = (int)i_Mesh->vTexCoords.size() -1 ;
	uvRightFront0[i_Sphere->Geodesic]  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomRight4[i_Sphere->Geodesic] = (int)i_Mesh->vTexCoords.size() -1 ;





	// Point Left ////////////////////////////////////////////////////
	newPoint = AN8XVECTOR3(-radius,0.0f,0.0f);
	i_Mesh->vPoints.push_back(newPoint);
	++i_Mesh->NbrPoint;
	TopLeft [i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	BottomLeft [i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	LeftBack[i_Sphere->Geodesic] = (int)i_Mesh->vPoints.size() -1 ;
	LeftFront[0]                 = (int)i_Mesh->vPoints.size() -1 ;

	newUV = AN8XVECTOR2(0.5f,0.5f);
	i_Mesh->vTexCoords.push_back(newUV);
	uvTopLeft2 [i_Sphere->Geodesic]  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvTopLeft3 [i_Sphere->Geodesic]  = (int)i_Mesh->vTexCoords.size() -1 ;

	uvLeftBack2[i_Sphere->Geodesic]  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvLeftFront3[0]                  = (int)i_Mesh->vTexCoords.size() -1 ;

	uvBottomLeft6 [i_Sphere->Geodesic]  = (int)i_Mesh->vTexCoords.size() -1 ;
	uvBottomLeft7 [i_Sphere->Geodesic]  = (int)i_Mesh->vTexCoords.size() -1 ;


	AN8XVECTOR3 vec3;

	///////////////////////////////////////////////////////////////
	// Create points on the edges of the initial octahedron
	for (int i = 1; i < i_Sphere->Geodesic; i++)
	{
		float t = i /(float) i_Sphere->Geodesic;
		float t1 = 1.0f - t;

		// TopFront ////////////////////////////////////////
		newPoint = AN8XVECTOR3(0.0f, t1, t);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		TopFront[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvTopFront0[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		uvTopFront3[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// TopRight /////////////////////////////////////////
		newPoint = AN8XVECTOR3(t, t1, 0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		TopRight[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		newUV.x = 1.0f;
		i_Mesh->vTexCoords.push_back(newUV);
		uvTopRight0[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		newUV.x = 0.0f;
		i_Mesh->vTexCoords.push_back(newUV);
		uvTopRight1[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// TopBack /////////////////////////////////////////
		newPoint = AN8XVECTOR3(0.0f, t1, -t);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		TopBack [i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvTopBack1[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		uvTopBack2[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// TopLeft /////////////////////////////////////////
		newPoint = AN8XVECTOR3(-t, t1, 0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		TopLeft [i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvTopLeft2[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		uvTopLeft3[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// BottomFront /////////////////////////////////////
		newPoint = AN8XVECTOR3(0.0f, -t1, t);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		BottomFront[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvBottomFront4[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		uvBottomFront7[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// BottomRight /////////////////////////////////////
		newPoint = AN8XVECTOR3(t, -t1, 0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		BottomRight[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		newUV.x = 1.0f;
		i_Mesh->vTexCoords.push_back(newUV);
		uvBottomRight4[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		newUV.x = 0.0f;
		i_Mesh->vTexCoords.push_back(newUV);
		uvBottomRight5[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// BottomBack /////////////////////////////////////
		newPoint = AN8XVECTOR3(0.0f, -t1, -t);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		BottomBack [i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvBottomBack5[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		uvBottomBack6[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// BottomLeft /////////////////////////////////////
		newPoint = AN8XVECTOR3(-t, -t1, 0.0f);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		BottomLeft [i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvBottomLeft6[i] = (int)i_Mesh->vTexCoords.size() -1 ;
		uvBottomLeft7[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// RightFront /////////////////////////////////////
		newPoint = AN8XVECTOR3(t, 0.0f, t1);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		RightFront[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvRightFront0[i] = (int)i_Mesh->vTexCoords.size() -1 ;


		// RightBack /////////////////////////////////////
		newPoint = AN8XVECTOR3(t1, 0.0f, -t);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		RightBack[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvRightBack1[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// LeftBack /////////////////////////////////////
		newPoint = AN8XVECTOR3(-t, 0.0f, -t1);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		LeftBack[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvLeftBack2[i] = (int)i_Mesh->vTexCoords.size() -1 ;

		// LeftFront /////////////////////////////////////
		newPoint = AN8XVECTOR3(-t1, 0.0f, t);
		i_Mesh->vPoints.push_back(newPoint);
		++i_Mesh->NbrPoint;
		LeftFront[i] = (int)i_Mesh->vPoints.size() -1 ;

		AN8XVec3Normalize(&vec3,&i_Mesh->vPoints[i_Mesh->NbrPoint-1]);
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
		uvLeftFront3[i] = (int)i_Mesh->vTexCoords.size() -1 ;
	}

	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		TopFront,TopRight,RightFront,
		uvTopFront0,uvTopRight0,uvRightFront0,
		true,i_Option);
	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		TopLeft,TopFront,LeftFront,
		uvTopLeft3,uvTopFront3,uvLeftFront3,
		true,i_Option);
	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		TopRight,TopBack,RightBack,
		uvTopRight1,uvTopBack1,uvRightBack1,
		true,i_Option);
	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		TopBack,TopLeft,LeftBack,
		uvTopBack2,uvTopLeft2,uvLeftBack2,
		true,i_Option);

	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		BottomFront,BottomRight,RightFront,
		uvBottomFront4,uvBottomRight4,uvRightFront0,
		false,i_Option);
	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		BottomLeft,BottomFront,LeftFront,
		uvBottomLeft7,uvBottomFront7,uvLeftFront3,
		false,i_Option);
	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		BottomRight,BottomBack,RightBack,
		uvBottomRight5,uvBottomBack5,uvRightBack1,
		false,i_Option);
	AN8XFacesOfGeodesicSphere(i_Mesh,i_Sphere,
		BottomBack,BottomLeft,LeftBack,
		uvBottomBack6,uvBottomLeft6,uvLeftBack2,
		false,i_Option);

	i_Mesh->NbrPoint = (int)i_Mesh->vPoints.size();
	i_Mesh->NbrFace  = (int)i_Mesh->vFaces.size();

	radius = i_Sphere->Diameter/2.0f;

	for( int i = 0 ; i < i_Mesh->NbrPoint ; ++i)
	{
		AN8XVec3Normalize(&i_Mesh->vPoints[i],&i_Mesh->vPoints[i]);

		if( i_Option == AN8_NORMAL)
		{
			i_Mesh->vNormals.push_back(i_Mesh->vPoints[i]);
			++i_Mesh->NbrNormal;
		}

		/*
		AN8XVECTOR2 newUV;AN8XVECTOR4 vec4;AN8XQUATERNION quat;
		AN8XMATRIX matrix;AN8XVECTOR3 vec3;
		AN8XVECTOR3 axisY = AN8XVECTOR3(0.0f,1.0f,0.0f);
		AN8XQuaternionRotationAxis(&quat,&axisY,AN8X_PI / 2.0f);
		AN8XMatrixRotationQuaternion( &matrix, &quat );
		AN8XVec3Transform( &vec4, &i_Mesh->vPoints[i],&matrix);
		vec3.x = vec4.x;vec3.y = vec4.y;vec3.z = vec4.z;
		AN8XSphericalUVFromNormal( &newUV, &vec3);
		i_Mesh->vTexCoords.push_back(newUV);
*/

		i_Mesh->vPoints[i] = i_Mesh->vPoints[i] * radius;

	}

	i_Mesh->NbrUVCoord = (int) i_Mesh->vTexCoords.size();

	return i_Mesh;


}


An8Mesh* AN8XMeshFromSphere( An8Mesh* i_Mesh, An8Sphere* i_Sphere, int i_Option  )
{
	if( i_Mesh != NULL || i_Sphere == NULL )
		return NULL;

	int faceMask = 4;
	if( i_Option == AN8_NORMAL )
	{
		faceMask = 6;
	}


	if( i_Sphere->Kind != 0 ) // geodesic sphere
	{
		return AN8XMeshFromGeodesicSphere( i_Mesh,i_Sphere,i_Option );
	}
	else if( i_Sphere->Kind == 0 ) // normal sphere
	{
		i_Mesh = new An8Mesh();
		i_Mesh->Name = i_Sphere->Name;
		i_Mesh->BaseOrigin = i_Sphere->BaseOrigin; // 0 0 0
		i_Mesh->BaseOrientation = i_Sphere->BaseOrientation; // 0 0 0

		i_Mesh->NbrMateriallist = 1;
		i_Mesh->vMatNumber.push_back(i_Sphere->MaterialID);

		AN8XVECTOR3 vec3Ref = AN8XVECTOR3(0.0f,i_Sphere->Diameter/2.0f,0.0f);
		AN8XVECTOR3 vec3;AN8XVECTOR4 vec4;AN8XQUATERNION quat;
		AN8XVECTOR3 axisX = AN8XVECTOR3(0.0f,0.0f,1.0f);
		AN8XVECTOR3 axisY = AN8XVECTOR3(0.0f,1.0f,0.0f);
		AN8XMATRIX matrix;

		// Vetical
		float stepAngleX = -AN8X_PI / i_Sphere->DivLat;
		float stepAngleY = 2.0f * AN8X_PI / i_Sphere->DivLong;


		for( int i = 1 ; i < i_Sphere->DivLat ; ++i )
		{
			AN8XQuaternionRotationAxis(&quat,&axisX,stepAngleX*(float)i);
			AN8XMatrixRotationQuaternion( &matrix, &quat );
			AN8XVec3Transform( &vec4, &vec3Ref,&matrix);
			vec3.x = vec4.x;
			vec3.y = vec4.y;
			vec3.z = vec4.z;

			for( int j = 0 ; j < i_Sphere->DivLong ; ++j )
			{
				AN8XQuaternionRotationAxis(&quat,&axisY,stepAngleY*(float)j);
				AN8XMatrixRotationQuaternion( &matrix, &quat );
				AN8XVec3Transform( &vec4, &vec3,&matrix);

				// Points
				AN8XVECTOR3 newPoint = AN8XVECTOR3(
					vec4.x,
					vec4.y,
					vec4.z
					);
				i_Mesh->vPoints.push_back(newPoint);
				++i_Mesh->NbrPoint;

				// TexCoord
				AN8XVECTOR2 newTexCoord = AN8XVECTOR2(j*(1.0f/i_Sphere->DivLong),1.0f - (i*(1.0f/i_Sphere->DivLat) ));
				i_Mesh->vTexCoords.push_back(newTexCoord);

				// Normals
				if( i_Option == AN8_NORMAL)
				{
					AN8XVec3Normalize(&newPoint,&newPoint);
					i_Mesh->vNormals.push_back(newPoint);
					++i_Mesh->NbrNormal;
				}
			}

		}

		// Top vertex /////////////////
		AN8XVECTOR3 top = AN8XVECTOR3(
				0.0f,
				(i_Sphere->Diameter/2.0f),
				0.0f
				);
		i_Mesh->vPoints.push_back(top);
		++i_Mesh->NbrPoint;
		// Normal
		if( i_Option == AN8_NORMAL)
		{
			AN8XVec3Normalize(&top,&top);
			i_Mesh->vNormals.push_back(top);
			++i_Mesh->NbrNormal;
		}
		// TexCoord
		//AN8XVECTOR2 newTexCoord = AN8XVECTOR2(j*(1.0f/i_Sphere->DivLong),1.0f);
		//i_Mesh->vTexCoords.push_back(newTexCoord);


		// Bottom vertex /////////////////////////
		AN8XVECTOR3 bottom = AN8XVECTOR3(
				0.0f,
				- (i_Sphere->Diameter/2.0f),
				0.0f
				);
		i_Mesh->vPoints.push_back(bottom);
		++i_Mesh->NbrPoint;
		// Normal
		if( i_Option == AN8_NORMAL)
		{
			AN8XVec3Normalize(&bottom,&bottom);
			i_Mesh->vNormals.push_back(bottom);
			++i_Mesh->NbrNormal;
		}

		// Faces ////////////////////////
		for( int i = 0 ; i < i_Sphere->DivLat-2 ; ++i )
		{
			for( int j = 0 ; j < i_Sphere->DivLong ; ++j )
			{
				An8Face newFace;
				newFace.NbrVertex = 4;
				newFace.Mask = faceMask;
				newFace.Material = 0;

				// 0
				An8FaceIndice id0;
				id0.Point = (i*i_Sphere->DivLong + j);
				id0.TexCoord = id0.Point;
				id0.Normal = id0.Point;

				// 1
				An8FaceIndice id1;
				id1.Point = (i*i_Sphere->DivLong + j)+1;
				if( j == i_Sphere->DivLong - 1 )
				{
					id1.Point = (i*i_Sphere->DivLong);

					// TexCoord
					AN8XVECTOR2 newTexCoord = i_Mesh->vTexCoords[id1.Point];
					newTexCoord.x = 1.0f;
					i_Mesh->vTexCoords.push_back(newTexCoord);
					id1.TexCoord = (int)i_Mesh->vTexCoords.size()-1;
				}
				else
				{
					id1.TexCoord = id1.Point;
				}
				id1.Normal = id1.Point;

				// 2
				An8FaceIndice id2;
				id2.Point = ( (i+1)*i_Sphere->DivLong + j)+1;
				if( j == i_Sphere->DivLong - 1 )
				{
					id2.Point = ( (i+1)*i_Sphere->DivLong);

					// TexCoord
					AN8XVECTOR2 newTexCoord = i_Mesh->vTexCoords[id2.Point];
					newTexCoord.x = 1.0f;
					i_Mesh->vTexCoords.push_back(newTexCoord);
					id2.TexCoord = (int)i_Mesh->vTexCoords.size()-1;
				}
				else
				{
					id2.TexCoord = id2.Point;
				}
				id2.Normal = id2.Point;

				// 3
				An8FaceIndice id3;
				id3.Point = ( (i+1)*i_Sphere->DivLong + j);
				id3.TexCoord = id3.Point;
				id3.Normal = id3.Point;

				newFace.vIndices.push_back(id0);
				newFace.vIndices.push_back(id1);
				newFace.vIndices.push_back(id2);
				newFace.vIndices.push_back(id3);

				i_Mesh->vFaces.push_back(newFace);

				++i_Mesh->NbrFace;
/*
				AN8XVECTOR3 newNormal;
				i_Mesh->NbrNormal = 1;
				newNormal.x = 0.0f;
				newNormal.y = 0.0f;
				newNormal.z = -1.0f;
				i_Mesh->vNormals.push_back(newNormal);
				*/
			}
		} // faces

		// Faces top/bottom ////////////////////////
		for( int i = 0 ; i < 2 ; ++i )
		{
			for( int j = 0 ; j < i_Sphere->DivLong ; ++j )
			{
				An8Face newFace;
				newFace.NbrVertex = 3;
				newFace.Mask = faceMask;
				newFace.Material = 0;

				// 0
				An8FaceIndice id0;
				id0.Point = (i*i_Sphere->DivLong*(i_Sphere->DivLat-2) + j);
				id0.TexCoord = id0.Point;
				id0.Normal   = id0.Point;

				// 1
				An8FaceIndice id1;
				id1.Point = (i*i_Sphere->DivLong*(i_Sphere->DivLat-2) + j)+1;
				if( j == i_Sphere->DivLong - 1 )
				{
					id1.Point = i*i_Sphere->DivLong*(i_Sphere->DivLat-2);

					// TexCoord
					AN8XVECTOR2 newTexCoord = i_Mesh->vTexCoords[id1.Point];
					newTexCoord.x = 1.0f;
					i_Mesh->vTexCoords.push_back(newTexCoord);
					id1.TexCoord = (int)i_Mesh->vTexCoords.size()-1;
				}
				else
				{
					id1.TexCoord = id1.Point;
				}
				id1.Normal = id1.Point;


				// Top TexCoord
				AN8XVECTOR2 newTexCoord = AN8XVECTOR2(j*(1.0f/i_Sphere->DivLong),1.0f-(float)i);
				i_Mesh->vTexCoords.push_back(newTexCoord);

				// 2
				An8FaceIndice id2;
				id2.Point = i_Sphere->DivLong*(i_Sphere->DivLat-1) + i;
				id2.TexCoord = (int)i_Mesh->vTexCoords.size()-1;
				id2.Normal = id2.Point;

				if( i != 0 )
				{
					newFace.vIndices.push_back(id0);
					newFace.vIndices.push_back(id1);
					newFace.vIndices.push_back(id2);
				}
				else
				{
					newFace.vIndices.push_back(id0);
					newFace.vIndices.push_back(id2);
					newFace.vIndices.push_back(id1);
				}

				i_Mesh->vFaces.push_back(newFace);

				++i_Mesh->NbrFace;

			}
		} // faces
		i_Mesh->NbrUVCoord = (int)i_Mesh->vTexCoords.size();
		return i_Mesh;
	}
	return NULL;
}
