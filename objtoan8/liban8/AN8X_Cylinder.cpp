// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// Web Site: texel3d.free.fr

#include"An8Loader.h"
#include<math.h>

An8Mesh* AN8XMeshFromCylinder( An8Mesh* i_Mesh, An8Cylinder* i_Cylinder, int i_Option  )
{
	if( i_Mesh != NULL || i_Cylinder == NULL )
		return NULL;

	int faceMask = 4;
	if( i_Option == AN8_NORMAL )
	{
		faceMask = 6;
	}

	i_Mesh = new An8Mesh();
	i_Mesh->Name            = i_Cylinder->Name;
	i_Mesh->BaseOrigin      = i_Cylinder->BaseOrigin;
	i_Mesh->BaseOrientation = i_Cylinder->BaseOrientation;

	i_Mesh->NbrMateriallist = 1;
	i_Mesh->vMatNumber.push_back(i_Cylinder->MaterialID);


	float angle = (2 * AN8X_PI ) / i_Cylinder->DivLong;
	AN8XVECTOR3 vec3Ref = AN8XVECTOR3(i_Cylinder->StartDiameter/2.0f,0.0f,0.0f);
	AN8XVECTOR3 vec3RefTop = AN8XVECTOR3(i_Cylinder->EndDiameter/2.0f,i_Cylinder->Length,0.0f);
	AN8XVECTOR3 axisY = AN8XVECTOR3(0.0f,1.0f,0.0f);
	AN8XVECTOR4 vec4,vec4Top;
	AN8XQUATERNION quat;
	AN8XMATRIX matrix;

	float heightStep = i_Cylinder->Length/(float)i_Cylinder->DivLat;

	int** VertexIndices;
	VertexIndices = new int*[i_Cylinder->DivLong];
	
	AN8XVECTOR2 newTexCoord = AN8XVECTOR2(0.0f,0.0f);
	for( int i = 0 ; i < i_Cylinder->DivLong ; ++i )
	{
		AN8XQuaternionRotationAxis(&quat,&axisY,angle*(float)i);
		AN8XMatrixRotationQuaternion( &matrix, &quat );
		AN8XVec3Transform( &vec4, &vec3Ref,&matrix);
		AN8XVec3Transform( &vec4Top, &vec3RefTop,&matrix);

		AN8XVECTOR4 vecCone = vec4Top - vec4;
		AN8XVec4Normalize(&vecCone,&vecCone);

		newTexCoord.x = (1.0f / i_Cylinder->DivLong)*(float)i;

		VertexIndices[i] = new int[i_Cylinder->DivLat+1]; 

		for( int j = 0 ; j <= i_Cylinder->DivLat ; ++j )
		{
			// point
			AN8XVECTOR3 newPoint = AN8XVECTOR3(
				vec4.x + vecCone.x*(j*heightStep),
				vec4.y + vecCone.y*(j*heightStep),
				vec4.z + vecCone.z*(j*heightStep)
			);
			i_Mesh->vPoints.push_back(newPoint);
			++i_Mesh->NbrPoint;

			// texcoord
			newTexCoord.y = (1.0f / i_Cylinder->DivLat)*(float)j;
			i_Mesh->vTexCoords.push_back(newTexCoord);

			// Normal
			if( i_Option == AN8_NORMAL)
			{
				newPoint.y = 0.0f;
				AN8XVec3Normalize(&newPoint,&newPoint);
				i_Mesh->vNormals.push_back(newPoint);
				++i_Mesh->NbrNormal;
			}

			VertexIndices[i][j] = i_Mesh->NbrPoint - 1;
		}
	}

	newTexCoord.x = 1.0f;
	for( int j = 0 ; j <= i_Cylinder->DivLat ; ++j )
	{
		// texcoord
		newTexCoord.y = (1.0f / i_Cylinder->DivLat)*(float)j;
		i_Mesh->vTexCoords.push_back(newTexCoord);
	}

	// Faces ///////////////////////////////////////////////

	An8FaceIndice index;

	// Face Bottom
	An8Face newFaceBottom;
	newFaceBottom.NbrVertex = i_Cylinder->DivLong;
	newFaceBottom.Mask      = faceMask;
	newFaceBottom.Material  = 0;

	if( i_Cylinder->CapStart == true )
	{
		// Disc base
		//for (int i = i_Cylinder->DivLong-1 ; i >= 0 ; --i )
		for (int i = 0 ; i < i_Cylinder->DivLong ; ++i )
		{
			index.Point  = VertexIndices[i][0];
			index.Normal = i_Mesh->NbrNormal; // To Do
			index.TexCoord = i*(i_Cylinder->DivLat+1);;
			newFaceBottom.vIndices.push_back(index);
		}
		i_Mesh->vFaces.push_back(newFaceBottom);

		if( i_Option == AN8_NORMAL)
		{
			AN8XVECTOR3 newNormal;
			newNormal.x = 0.0f;
			newNormal.y = -1.0f;
			newNormal.z = 0.0f;
			i_Mesh->vNormals.push_back(newNormal);
			++i_Mesh->NbrNormal;
		}
	}

	if( i_Cylinder->CapEnd == true )
	{
		// Disc Top
		An8Face newFaceTop;
		newFaceTop.NbrVertex = i_Cylinder->DivLong;
		newFaceTop.Mask      = faceMask;
		newFaceTop.Material  = 0;
		//for (int i = 0 ; i < i_Cylinder->DivLong ; ++i )
		for (int i = i_Cylinder->DivLong-1 ; i >= 0 ; --i )
		{
			index.Point  = VertexIndices[i][i_Cylinder->DivLat];
			index.Normal = i_Mesh->NbrNormal; // To Do
			index.TexCoord = i*(i_Cylinder->DivLat+1);
			newFaceTop.vIndices.push_back(index);
		}
		i_Mesh->vFaces.push_back(newFaceTop);

		if( i_Option == AN8_NORMAL)
		{
			AN8XVECTOR3 newNormal;
			newNormal.x = 0.0f;
			newNormal.y = 1.0f;
			newNormal.z = 0.0f;
			i_Mesh->vNormals.push_back(newNormal);
			++i_Mesh->NbrNormal;
		}
	}


	// Faces of the "tunnel"


	for( int i = 0 ; i < i_Cylinder->DivLong ; ++i )
	{
		for( int j = 0 ; j < i_Cylinder->DivLat ; ++j )
		{
			An8Face newFaceTunnel;
			newFaceTunnel.NbrVertex = 4;
			newFaceTunnel.Mask      = faceMask;
			newFaceTunnel.Material  = 0;

			if( i != i_Cylinder->DivLong-1 )
			{
				index.Point  = VertexIndices[i][j];
				index.Normal = index.Point;
				index.TexCoord = j + i*(i_Cylinder->DivLat+1);
				newFaceTunnel.vIndices.push_back(index);

				index.Point  = VertexIndices[i][j+1];
				index.Normal = index.Point;
				index.TexCoord = (j+1) + i*(i_Cylinder->DivLat+1);
				newFaceTunnel.vIndices.push_back(index);

				index.Point  = VertexIndices[i+1][j+1];
				index.Normal = index.Point;
				index.TexCoord = (j+1) + (i+1)*(i_Cylinder->DivLat+1);
				newFaceTunnel.vIndices.push_back(index);

				index.Point  = VertexIndices[i+1][j];
				index.Normal = index.Point;
				index.TexCoord = j + (i+1)*(i_Cylinder->DivLat+1);
				newFaceTunnel.vIndices.push_back(index);
			}
			else
			{
				index.Point    = VertexIndices[i][j];
				index.Normal   = index.Point;
				index.TexCoord = j + i*(i_Cylinder->DivLat+1);
				newFaceTunnel.vIndices.push_back(index);

				index.Point    = VertexIndices[i][j+1];
				index.Normal   = index.Point;
				index.TexCoord = (j+1) + i*(i_Cylinder->DivLat+1);
				newFaceTunnel.vIndices.push_back(index);

				index.Point    = VertexIndices[0][j+1];
				index.Normal   = index.Point;
				index.TexCoord = (j+1) + (i+1)*(i_Cylinder->DivLat+1);;
				newFaceTunnel.vIndices.push_back(index);

				index.Point    = VertexIndices[0][j];
				index.Normal   = index.Point;
				index.TexCoord = (j) + (i+1)*(i_Cylinder->DivLat+1);;
				newFaceTunnel.vIndices.push_back(index);
			}
			i_Mesh->vFaces.push_back(newFaceTunnel);

		}
	}

	for( int i = 0 ; i < i_Cylinder->DivLong ; ++i )
	{
		delete [] VertexIndices[i];
		VertexIndices[i] = NULL;
	}
	delete [] VertexIndices;

	i_Mesh->NbrUVCoord = (int)i_Mesh->vTexCoords.size();
	i_Mesh->NbrFace = (int)i_Mesh->vFaces.size();
	return i_Mesh;

}

