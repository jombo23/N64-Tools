// This file in a part of LibAn8, Copyright (c) 2002-2010, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "AN8X.h"
#include "An8loader.h"

#include<math.h>
#include <algorithm>

AN8XBone::AN8XBone(void)
{
	Length=0;Parent = 0;Parent= NULL;
	ParentID = -1;
	HasInfluence = false;
}

AN8XBone::~AN8XBone()
{
	Parent= NULL;
}

AN8XVertexWeights::AN8XVertexWeights()
{
	NbrWeights = 0;
	WeightsBoneIDs[0] = 0;
	WeightsBoneIDs[1] = 0;
	WeightsBoneIDs[2] = 0;
	WeightsBoneIDs[3] = 0;
	Weights[0] = 0.0f;
	Weights[1] = 0.0f;
	Weights[2] = 0.0f;
	Weights[3] = 0.0f;
}


void AN8XBone::ComputeInfluenceData(An8Influence* i_Influence)
{
	// Compute data needed to compute if a vertex is in an influence area

	AN8XVECTOR3 vec3BoneDirection;
	vec3BoneDirection = BoneEndPosition - BoneStartPosition;
	float boneLength = AN8XVec3Length(&vec3BoneDirection);
	AN8XVec3Normalize(&vec3BoneDirection,&vec3BoneDirection);

	// Radius
	Influence.StartCenterInRadius = i_Influence->InRadiusStart;
	Influence.StartCenterOutRadius = i_Influence->OutRadiusStart;

	Influence.EndCenterInRadius    = i_Influence->InRadiusEnd;
	Influence.EndCenterOutRadius   = i_Influence->OutRadiusEnd;

	// Sphere of influence center
	//i_Influence->Center0 between [-0.5,0.5]
	//i_Influence->Center1 between [0.5,1.5]
	Influence.StartCenter =  BoneStartPosition + vec3BoneDirection * (boneLength*i_Influence->CenterStart);
	Influence.EndCenter   =  BoneStartPosition + vec3BoneDirection * (boneLength*i_Influence->CenterEnd);

	AN8XVECTOR3 vec3 = Influence.EndCenter - Influence.StartCenter;
	float influenceLength = AN8XVec3Length(&vec3);
}



float AN8XBone::ProjectPointToBone(AN8XVECTOR3* o_Proj,AN8XVECTOR3* i_Vertex)
{
	AN8XVECTOR3 vec3AToVertex;
	vec3AToVertex = *i_Vertex - this->BoneStartPosition;
	float lengthAToVertex = AN8XVec3Length(&vec3AToVertex);

	// Bone Direction Normalized
	AN8XVECTOR3 vec3AToB;
	vec3AToB = this->BoneEndPosition - this->BoneStartPosition;
	AN8XVec3Normalize(&vec3AToB,&vec3AToB);

	// Start To Vertex Normalized
	AN8XVec3Normalize(&vec3AToVertex,&vec3AToVertex);

	// Angle
	float coseAngle = AN8XVec3Dot(&vec3AToVertex,&vec3AToB);

	float distanceAToProjVertex = lengthAToVertex*coseAngle;

	*o_Proj = this->BoneStartPosition + (vec3AToB*distanceAToProjVertex);


	AN8XVECTOR3 vec3StartToVertex;
	vec3StartToVertex = *i_Vertex - this->Influence.StartCenter;
	float lengthStartToVertex = AN8XVec3Length(&vec3StartToVertex);
	AN8XVec3Normalize(&vec3StartToVertex,&vec3StartToVertex);
	coseAngle = AN8XVec3Dot(&vec3StartToVertex,&vec3AToB);
	float distStartInfluToProjVert = lengthStartToVertex*coseAngle;

	return distStartInfluToProjVert;
}


// Compute the influence factor of the bone on the vertex
// Return true or false if the vertex is influenced by bone or not
bool AN8XBone::IsVertexInfluenced(float* i_vertex,float* o_influence)
{
	if( HasInfluence == false )
		return false;

	AN8XVECTOR3 vertex(i_vertex[0],i_vertex[1],i_vertex[2]);

	AN8XVECTOR3 vec3 = Influence.EndCenter - Influence.StartCenter;
	float influenceLength = AN8XVec3Length(&vec3);

	// Start to vertex
	AN8XVECTOR3 vec3StartToVertex;
	vec3StartToVertex = vertex - Influence.StartCenter;
	float lengthVec3StartToVertex = AN8XVec3Length(&vec3StartToVertex);

	// End to vertex
	AN8XVECTOR3 vec3EndToVertex;
	vec3EndToVertex = vertex - Influence.EndCenter;
	float lengthVec3EndToVertex = AN8XVec3Length(&vec3EndToVertex);

	// Get vertex projected on bone segment
	AN8XVECTOR3 vertexOnBone;
	float distFromStart    = ProjectPointToBone(&vertexOnBone,&vertex);
	//float distFromStart    = UtilProjectPointToSegment(&vertexOnBone,&vertex,&Influence.StartCenter,&Influence.EndCenter);
	float distFromStartPer = 0.0f;
	if( distFromStart > 0.00001f || distFromStart < -0.00001f ) // avoid division by zero
		distFromStartPer = distFromStart / influenceLength; 

	vec3 = vertex - vertexOnBone;
	float distance = AN8XVec3Length(&vec3);

	// Inner ///////////


	if( distFromStart >= 0.0f && distFromStart <= influenceLength ) 
	{
		float inDistance  = Influence.EndCenterInRadius*distFromStartPer + Influence.StartCenterInRadius*(1.0f-distFromStartPer);
		float outDistance = Influence.EndCenterOutRadius*distFromStartPer + Influence.StartCenterOutRadius*(1.0f-distFromStartPer);

		if( distance < inDistance )
		{
			*o_influence = 1.0f;
			return true;
		}
		else if( distance < outDistance )
		{
			float InOut = outDistance - inDistance;
			*o_influence = (distance-inDistance)/InOut ;
			return true;
		}
	}
	else if( lengthVec3StartToVertex <= Influence.StartCenterInRadius) // In Inner sphere of start
	{
		*o_influence = 1.0f;
		return true;
	}
	else if( lengthVec3EndToVertex <= Influence.EndCenterInRadius  )// In Inner sphere of end
	{
		*o_influence = 1.0f;
		return true;
	}
	else if ( lengthVec3StartToVertex <= Influence.StartCenterOutRadius) // Between Inner and outer sphere of start
	{
		float InOut = Influence.StartCenterOutRadius - Influence.StartCenterInRadius;
		*o_influence = (lengthVec3StartToVertex-Influence.StartCenterInRadius)/InOut ;
		return true;
	}
	else if ( lengthVec3EndToVertex <= Influence.EndCenterOutRadius) // Between Inner and outer sphere of end
	{
		float InOut = Influence.EndCenterOutRadius - Influence.EndCenterInRadius;
		*o_influence = (lengthVec3EndToVertex-Influence.EndCenterInRadius)/InOut ;
		return true;
	}
	return false;
}



// function used by std::sort to sort key using the frame order
bool AN8XSequence::SortKeys(AN8XFloatKey i,AN8XFloatKey j) 
{ 
	return (i.KeyFrame<j.KeyFrame); 
}


bool AN8XSequence::LoadSequence(std::vector<AN8XBone> *i_BoneList,An8Sequence* i_pAn8Sequence,int i_FigureID)
{
	this->Name		= i_pAn8Sequence->Name;
	this->FigureName = i_pAn8Sequence->FigureName;
	this->NbrFrame	= i_pAn8Sequence->NbrFrame;

	int figureID = i_FigureID;

	this->vBoneTracks.resize(i_BoneList->size());

	for( int joint = 0; joint < i_pAn8Sequence->NbrJointAngle; ++joint )
	{
		for( int bone = 0; bone < (int)i_BoneList->size(); ++bone)
		{
			if( i_BoneList[0][bone].Name.compare(i_pAn8Sequence->vJointAngles[joint].BoneName) == 0 )
			{
				if( i_pAn8Sequence->vJointAngles[joint].HasTrack == true )
				{

					for( int floatKey = 0; floatKey < (int)i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys.size(); ++floatKey)
					{
						int keyFrame = i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys[floatKey].FrameNumber;
						int sameKeyIndex = -1;
						for( int k = 0; k < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++k)
						{
							if( this->vBoneTracks[bone].vFloatKeys[k].KeyFrame == keyFrame)
							{
								sameKeyIndex = k;
								break;
							}
						}
						if( sameKeyIndex > -1 )
						{
							if(i_pAn8Sequence->vJointAngles[joint].Axe == 1 ) // X
							{
								if( this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationX.size() > 0 )
								{
									this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationX.clear();
								}
								this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationX.push_back(float(i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys[floatKey].vFloats[0]));
							}
							else if(i_pAn8Sequence->vJointAngles[joint].Axe == 2 ) // Y
							{
								if( this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationY.size() > 0 )
								{
									this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationY.clear();
								}
								this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationY.push_back(float(i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys[floatKey].vFloats[0]));
							}
							else if(i_pAn8Sequence->vJointAngles[joint].Axe == 3 ) // Z
							{
								if( this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationZ.size() > 0 )
								{
									this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationZ.clear();
								}
								this->vBoneTracks[bone].vFloatKeys[sameKeyIndex].RotationZ.push_back(float(i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys[floatKey].vFloats[0]));
							}
							continue;
						}


						AN8XFloatKey newFloatKey;
						newFloatKey.KeyFrame = keyFrame;
						if(i_pAn8Sequence->vJointAngles[joint].Axe == 1 ) // X
						{
							newFloatKey.RotationX.push_back(float(i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys[floatKey].vFloats[0]));
						}
						else if(i_pAn8Sequence->vJointAngles[joint].Axe == 2 ) // Y
						{
							newFloatKey.RotationY.push_back(float(i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys[floatKey].vFloats[0]));
						}
						else if(i_pAn8Sequence->vJointAngles[joint].Axe == 3 ) // Z
						{
							newFloatKey.RotationZ.push_back(float(i_pAn8Sequence->vJointAngles[joint].Track.vFloatKeys[floatKey].vFloats[0]));
						}
						this->vBoneTracks[bone].vFloatKeys.push_back(newFloatKey);
					}

				}
			}
		}
	}

	// It should be only one vFloatKey for a given key
	// Now we must fill vFloatKeys to have X,Y,Z rotation for each key
	for( int bone = 0; bone < (int)this->vBoneTracks.size() ; ++bone)
	{
		for( int keys = 0; keys < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++keys)
		{
			//int frameCurrent = i_pSequence->vBoneTracks[bone].vFloatKeys[keys].KeyFrame;
			float before = 0.0f;int frameBefore = 0;
			float after = 0.0f;int frameAfter = 0;

			if( this->vBoneTracks[bone].vFloatKeys[keys].RotationX.size() > 0 )
			{
				// We have X, Look for the others
				if( this->vBoneTracks[bone].vFloatKeys[keys].RotationY.size() == 0 )
				{
					if( keys > 0 ) // all prevouis rotation have been computed 
					{
						before = this->vBoneTracks[bone].vFloatKeys[keys-1].RotationY[0];
						frameBefore = this->vBoneTracks[bone].vFloatKeys[keys-1].KeyFrame;
					}
					for( int nextValidKey = keys; nextValidKey < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++nextValidKey)
					{
						if( this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationY.size() > 0)
						{
							after = this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationY[0];
							frameAfter = this->vBoneTracks[bone].vFloatKeys[nextValidKey].KeyFrame;
							break;
						}
					}
					// Compute Y: Interpolation between before and after (use frame delta)
					int deltaFrame = frameAfter - frameBefore;
					float value = 0.0f;
					if( deltaFrame == 0 )
						value = before;
					else
						value = before + float(deltaFrame)*(after-before)/float(frameAfter-frameBefore);
					this->vBoneTracks[bone].vFloatKeys[keys].RotationY.push_back(float(value));
				}
				if( this->vBoneTracks[bone].vFloatKeys[keys].RotationZ.size() == 0 )
				{
					if( keys > 0 ) // all prevouis rotation have been computed 
					{
						before = this->vBoneTracks[bone].vFloatKeys[keys-1].RotationZ[0];
						frameBefore = this->vBoneTracks[bone].vFloatKeys[keys-1].KeyFrame;
					}
					for( int nextValidKey = keys; nextValidKey < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++nextValidKey)
					{
						if( this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationZ.size() > 0)
						{
							after = this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationZ[0];
							frameAfter = this->vBoneTracks[bone].vFloatKeys[nextValidKey].KeyFrame;
							break;
						}
					}
					// Compute Y: Interpolation between before and after (use frame delta)
					int deltaFrame = frameAfter - frameBefore;
					float value = 0.0f;
					if( deltaFrame == 0 )
						value = before;
					else
						value = before + float(deltaFrame)*(after-before)/float(frameAfter-frameBefore);
					this->vBoneTracks[bone].vFloatKeys[keys].RotationZ.push_back(float(value));
				}
			}
			if( this->vBoneTracks[bone].vFloatKeys[keys].RotationY.size() > 0 )
			{
				// We have Y, Look for the others
				if( this->vBoneTracks[bone].vFloatKeys[keys].RotationX.size() == 0 )
				{
					if( keys > 0 ) // all prevouis rotation have been computed 
					{
						before = this->vBoneTracks[bone].vFloatKeys[keys-1].RotationX[0];
						frameBefore = this->vBoneTracks[bone].vFloatKeys[keys-1].KeyFrame;
					}
					for( int nextValidKey = keys; nextValidKey < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++nextValidKey)
					{
						if( this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationX.size() > 0)
						{
							after = this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationX[0];
							frameAfter = this->vBoneTracks[bone].vFloatKeys[nextValidKey].KeyFrame;
							break;
						}
					}
					// Compute Y: Interpolation between before and after (use frame delta)
					int deltaFrame = frameAfter - frameBefore;
					float value = 0.0f;
					if( deltaFrame == 0 )
						value = before;
					else
						value = before + float(deltaFrame)*(after-before)/float(frameAfter-frameBefore);
					this->vBoneTracks[bone].vFloatKeys[keys].RotationX.push_back(float(value));
				}
				if( this->vBoneTracks[bone].vFloatKeys[keys].RotationZ.size() == 0 )
				{
					if( keys > 0 ) // all prevouis rotation have been computed 
					{
						before = this->vBoneTracks[bone].vFloatKeys[keys-1].RotationZ[0];
						frameBefore = this->vBoneTracks[bone].vFloatKeys[keys-1].KeyFrame;
					}
					for( int nextValidKey = keys; nextValidKey < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++nextValidKey)
					{
						if( this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationZ.size() > 0)
						{
							after = this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationZ[0];
							frameAfter = this->vBoneTracks[bone].vFloatKeys[nextValidKey].KeyFrame;
							break;
						}
					}
					// Compute Y: Interpolation between before and after (use frame delta)
					int deltaFrame = frameAfter - frameBefore;
					float value = 0.0f;
					if( deltaFrame == 0 )
						value = before;
					else
						value = before + float(deltaFrame)*(after-before)/float(frameAfter-frameBefore);
					this->vBoneTracks[bone].vFloatKeys[keys].RotationZ.push_back(float(value));
				}
			}
			if( this->vBoneTracks[bone].vFloatKeys[keys].RotationZ.size() > 0 )
			{
				// We have Z, Look for the others
				if( this->vBoneTracks[bone].vFloatKeys[keys].RotationX.size() == 0 )
				{
					if( keys > 0 ) // all prevouis rotation have been computed 
					{
						before = this->vBoneTracks[bone].vFloatKeys[keys-1].RotationX[0];
						frameBefore = this->vBoneTracks[bone].vFloatKeys[keys-1].KeyFrame;
					}
					for( int nextValidKey = keys; nextValidKey < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++nextValidKey)
					{
						if( this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationX.size() > 0)
						{
							after = this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationX[0];
							frameAfter = this->vBoneTracks[bone].vFloatKeys[nextValidKey].KeyFrame;
							break;
						}
					}
					// Compute Y: Interpolation between before and after (use frame delta)
					int deltaFrame = frameAfter - frameBefore;
					float value = 0.0f;
					if( deltaFrame == 0 )
						value = before;
					else
						value = before + float(deltaFrame)*(after-before)/float(frameAfter-frameBefore);
					this->vBoneTracks[bone].vFloatKeys[keys].RotationX.push_back(float(value));
				}
				if( this->vBoneTracks[bone].vFloatKeys[keys].RotationY.size() == 0 )
				{
					if( keys > 0 ) // all prevouis rotation have been computed 
					{
						before = this->vBoneTracks[bone].vFloatKeys[keys-1].RotationY[0];
						frameBefore = this->vBoneTracks[bone].vFloatKeys[keys-1].KeyFrame;
					}
					for( int nextValidKey = keys; nextValidKey < (int)this->vBoneTracks[bone].vFloatKeys.size(); ++nextValidKey)
					{
						if( this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationY.size() > 0)
						{
							after = this->vBoneTracks[bone].vFloatKeys[nextValidKey].RotationY[0];
							frameAfter = this->vBoneTracks[bone].vFloatKeys[nextValidKey].KeyFrame;
							break;
						}
					}
					// Compute Y: Interpolation between before and after (use frame delta)
					int deltaFrame = frameAfter - frameBefore;
					float value = 0.0f;
					if( deltaFrame == 0 )
						value = before;
					else
						value = before + float(deltaFrame)*(after-before)/float(frameAfter-frameBefore);
					this->vBoneTracks[bone].vFloatKeys[keys].RotationY.push_back(float(value));
				}
			}
		} // end of key

		sort (this->vBoneTracks[bone].vFloatKeys.begin(), this->vBoneTracks[bone].vFloatKeys.end(), SortKeys);
	} // End of bone

	this->ComputeFloatKeysQuaternions();
	return true;
}


void AN8XSequence::ComputeFloatKeysQuaternions(void)
{
	for( int t = 0; t < (int)vBoneTracks.size(); ++t)
	{
		for( int k = 0; k < (int)vBoneTracks[t].vFloatKeys.size(); ++k)
		{

			AN8XQuaternionRotationYawPitchRoll(
				&vBoneTracks[t].vFloatKeys[k].Orientation,
				AN8XToRadian( vBoneTracks[t].vFloatKeys[k].RotationY[0]),
				AN8XToRadian( vBoneTracks[t].vFloatKeys[k].RotationX[0]),
				AN8XToRadian( vBoneTracks[t].vFloatKeys[k].RotationZ[0])
				);
		}
	}
}


AN8XQUATERNION* AN8XSequence::GetBoneOrientationAtFrame(AN8XQUATERNION* o_quat,int i_Bone, int i_Frame)
{
	if( i_Bone > (int) vBoneTracks.size() )
		return NULL;

	int previousKey = -1;
	int previousFrame = 0;
	for( int k = 0; k < (int)vBoneTracks[i_Bone].vFloatKeys.size(); ++k)
	{
		if( vBoneTracks[i_Bone].vFloatKeys[k].KeyFrame <= i_Frame )
		{
			previousKey   = k;
			previousFrame = vBoneTracks[i_Bone].vFloatKeys[k].KeyFrame;
		}
	}

	if( previousKey == -1 )
		return NULL;


	if( previousKey+1 >= (int)vBoneTracks[i_Bone].vFloatKeys.size() )
	{
		// This is the last key of sequence, no interpolation needed
		*o_quat = vBoneTracks[i_Bone].vFloatKeys[previousKey].Orientation;
	}
	else
	{
		int nextKey = previousKey+1;
		int nextFrame = vBoneTracks[i_Bone].vFloatKeys[nextKey].KeyFrame;

		float t = float(i_Frame - previousFrame) / float(nextFrame - previousFrame);

		AN8XQuaternionSlerp(
			o_quat, 
			&vBoneTracks[i_Bone].vFloatKeys[previousKey].Orientation, 
			&vBoneTracks[i_Bone].vFloatKeys[nextKey].Orientation, 
			t);

	}
	return o_quat;

}


void AN8XObjectInstanceInFigure::ComputePaintedWeights(std::vector<AN8XBone>* vBones)
{
	// Comment faire pour que l'ordre des meshes pour les poids soit celui de l'objet et non pas
	// celui des MeshName des named object ?

	// This vector will help to get the real boneID of the weight
	std::vector<int> boneIDsMap;
	for( int wb = 0 ; wb < (int)NamedObjectPtr->vWeightedBy.size(); ++wb)
	{
		boneIDsMap.push_back(0);
		for(int bone = 0; bone < (int)vBones->size(); ++bone)
		{
			if( vBones->at(bone).Name.compare(NamedObjectPtr->vWeightedBy[wb]) == 0 )
			{
				boneIDsMap[(int)boneIDsMap.size()-1] = bone;
			}
		}
	}



	

	for(int mesh = 0; mesh < ObjectPtr->NbrMesh; ++mesh)
	{
		MeshWeights newMeshWeights;
		vMeshesWeights.push_back(newMeshWeights);
		vMeshesWeights[vMeshesWeights.size()-1].HasMeshPaintedWeights = false;

		for(int pt = 0; pt < ObjectPtr->vMeshes[mesh].NbrPoint; ++pt) 
		{
			AN8XVertexWeights newVertexWeights;
			vMeshesWeights[vMeshesWeights.size()-1].vVerticesWeights.push_back(newVertexWeights);
		}

		for(int w = 0; w < (int)NamedObjectPtr->vWeights.size(); ++w)
		{
			if( ObjectPtr->vMeshes[mesh].Name.compare(NamedObjectPtr->vWeights[w].MeshName) == 0 )
			{
				// Compute weight of the mesh

				vMeshesWeights[vMeshesWeights.size()-1].HasMeshPaintedWeights = true;

				// If the number of vertex in vertexWeights is different from the number of vertex in
				// the mesh, there is a problem.
				if( (int)NamedObjectPtr->vWeights[w].vVertexWeights.size() > ObjectPtr->vMeshes[mesh].NbrPoint )
				{
					continue;
				}

				for( int v = 0; v < (int)NamedObjectPtr->vWeights[w].vVertexWeights.size(); ++v)
				{
					AN8XVertexWeights* pVertexWeight = &vMeshesWeights[vMeshesWeights.size()-1].vVerticesWeights[v];
					pVertexWeight->NbrWeights = NamedObjectPtr->vWeights[w].vVertexWeights[v].NbrOfWeightValue;

					for( int b = 0; b < NamedObjectPtr->vWeights[w].vVertexWeights[v].NbrOfWeightValue; ++b)
					{
						if( b > 3 )
							break;

						int boneID = NamedObjectPtr->vWeights[w].vVertexWeights[v].vWeightValues[b].BoneIndex;
						pVertexWeight->WeightsBoneIDs[b] = boneIDsMap[boneID];
						pVertexWeight->Weights[b]		   = NamedObjectPtr->vWeights[w].vVertexWeights[v].vWeightValues[b].Weight;
						//vMeshesWeights[vMeshesWeights.size()-1].vVerticesWeights.push_back(newVertexWeights);
					}
				}
			}
		}
	}

}

void AN8XObjectInstanceInFigure::ComputeInfluences(std::vector<AN8XBone>* vBones)
{
	for( int m = 0; m < ObjectPtr->NbrMesh; ++m)
	{
		if( vMeshesWeights[m].HasMeshPaintedWeights == true )
			continue;

		for( int v = 0; v < ObjectPtr->vMeshes[m].NbrPoint; ++v)
		{
			// Compute vertex weights from influence

			// Get influence values
			float influenceValue[4];
			influenceValue[0] = 0.0f;
			influenceValue[1] = 0.0f;
			influenceValue[2] = 0.0f;
			influenceValue[3] = 0.0f;
			int bones[4];
			bones[0] = 0;
			bones[1] = 0;
			bones[2] = 0;
			bones[3] = 0;
			int nbrInfluence = 0;
			for( int b = 0; b < (int)vBones->size(); ++b)
			{
				AN8XVECTOR3 vec3 = ObjectPtr->vMeshes[m].vPoints[v];
				if( vBones->at(b).IsVertexInfluenced((float*)&vec3,&influenceValue[nbrInfluence]) == true )
				{
					// influenceValue has been stored, we need the bone ID
					//influenceValue[nbrInfluence] = 1.0f;
					bones[nbrInfluence] = b;
					++nbrInfluence;
				}
				if( nbrInfluence > 3 ) // Maximum 3 weights (index 0,1,2,3)
					break;
			}

			// Compute final weights
			float totalInfluence = 0.0f;
			for( int i = 0; i < nbrInfluence; ++i)
			{
				totalInfluence = totalInfluence + influenceValue[i];
			}
			for( int i = 0; i < nbrInfluence; ++i)
			{
				vMeshesWeights[m].vVerticesWeights[v].NbrWeights		= nbrInfluence;
				vMeshesWeights[m].vVerticesWeights[v].Weights[i]        = influenceValue[i] / totalInfluence;
				vMeshesWeights[m].vVerticesWeights[v].WeightsBoneIDs[i] = bones[i];
			}
		}
	}
}


void AN8XFigureHelper::ProcessNode(An8File* i_File,
					 std::vector<AN8XBone>* boneList,
					 std::vector<AN8XObjectInstanceInFigure*>* objectInstance,
					 std::vector<An8Mesh*>* figureMeshesList, // Must be deleted by you
					  std::vector<int>* figureMeshesListBoneIDs,
					  std::vector<AN8XMATRIX>* figureMeshesListMatrices,
					  std::vector<An8Material*>* figureMeshesListMaterials, // Must be deleted by you
					 int i_An8FigureID,An8Bone* Root,
					 AN8XMATRIX* i_ParentAbsoluteMatrix)
{
	int i_CurrentFigure = 0;
	AN8XBone newBone;
	newBone.Name = Root->Name;
	newBone.Orientation.x = Root->Orientation.x;
	newBone.Orientation.y = Root->Orientation.y;
	newBone.Orientation.z = Root->Orientation.z;
	newBone.Orientation.w = Root->Orientation.w;
	newBone.ParentName = Root->ParentName;
	newBone.Length = Root->Length;

	double yaw,pitch,roll;
	yaw = pitch = roll = 0.0f;
	AN8XYawPitchRollQuaternion(&yaw,&pitch,&roll,&Root->Orientation);
	//newBone.Rotation = AN8XVECTOR3(float(yaw),float(pitch),float(roll));
	newBone.Rotation = AN8XVECTOR3(float(pitch),float(yaw),float(roll)); // XXX

	// Compute Absolute matrix /////////////////////////////////////
	AN8XMATRIX boneMatrix;
	AN8XMatrixRotationQuaternion( &boneMatrix , &Root->Orientation );
	AN8XMATRIX boneTranslationMatrix;
	boneTranslationMatrix._41 = 0.0f; // x
	boneTranslationMatrix._42 = Root->Length; // y
	boneTranslationMatrix._43 = 0.0f; // z
	AN8XMatrixMultiply(&boneMatrix,&boneMatrix,&boneTranslationMatrix);
	AN8XMATRIX absoluteMatrix;
	AN8XMatrixMultiply(&absoluteMatrix,i_ParentAbsoluteMatrix,&boneMatrix);

	// Compute Bone Bind Position ////////////////////////////
	newBone.BoneStartPosition = AN8XVECTOR3(0.0f,0.0f,0.0f);
	newBone.BoneEndPosition = AN8XVECTOR3(0.0f,Root->Length,0.0f);
	AN8XVECTOR4 vec4;
	AN8XVec3Transform(&vec4,&newBone.BoneStartPosition,i_ParentAbsoluteMatrix);
	newBone.BoneStartPosition.x = vec4.x;
	newBone.BoneStartPosition.y = vec4.y;
	newBone.BoneStartPosition.z = vec4.z;

	AN8XVECTOR3 vec3Test(0.0f,0.0f,0.0f);
	AN8XVec3Transform(&vec4,&vec3Test,&absoluteMatrix);
	newBone.BoneEndPosition.x = vec4.x;
	newBone.BoneEndPosition.y = vec4.y;
	newBone.BoneEndPosition.z = vec4.z;
	

	newBone.HasInfluence = Root->HasInfluence;
	if( newBone.HasInfluence == true )
		newBone.ComputeInfluenceData(&Root->Influence);
	///////////////////////////////////////////////////////////

	//for( int i = 0 ; i < Root->NbrNamedObject ; ++i )
	for( int i = 0 ; i < Root->NbrNamedObject ; ++i )
	{
		for( int obj = 0; obj < i_File->NbrObject ; ++obj)
		{
			if( i_File->vObjects[obj].Name.compare(Root->vNamedObjects[i].ObjectName) == 0 )
			{	
				AN8XObjectInstanceInFigure* newObjInstance = new AN8XObjectInstanceInFigure();
				objectInstance->push_back(newObjInstance);

				newObjInstance->ObjectPtr = &i_File->vObjects[obj];
				newObjInstance->BoneID = (int)(*boneList).size();
				newObjInstance->NamedObjectPtr = &Root->vNamedObjects[i];

				/* Matrix. We must:
				1) Rotate vertices with the current relative bone rotation matrix
				2) Transform the result with the parent matrix
				*/
				AN8XMATRIX boneRotationMatrix;
				AN8XMatrixRotationQuaternion( &boneRotationMatrix , &Root->Orientation );
				AN8XMATRIX matrix;
				AN8XMatrixMultiply(&matrix,i_ParentAbsoluteMatrix,&boneRotationMatrix);
				// Add Local translation
				AN8XMATRIX localTranslationMatrix;
				localTranslationMatrix._41 = Root->vNamedObjects[i].Origin.x;
				localTranslationMatrix._42 = Root->vNamedObjects[i].Origin.y;
				localTranslationMatrix._43 = Root->vNamedObjects[i].Origin.z;
				AN8XMatrixMultiply(&matrix,&matrix,&localTranslationMatrix);
				// Add local rotation
				AN8XMATRIX localRotationMatrix;
				AN8XMatrixRotationQuaternion( &localRotationMatrix , &Root->vNamedObjects[i].Orientation );
				AN8XMatrixMultiply(&matrix,&matrix,&localRotationMatrix);
				newObjInstance->Matrix = matrix;

				// Material
				for( int mesh = 0; mesh < newObjInstance->ObjectPtr->NbrMesh;++mesh)
				{
					bool materialFound = false;
					if( newObjInstance->ObjectPtr->vMeshes[mesh].vMatNumber.size() > 0 )
					{
						if( newObjInstance->ObjectPtr->vMeshes[mesh].vFaces.size() > 0 )
						{
							int matNumber = newObjInstance->ObjectPtr->vMeshes[mesh].vMatNumber[newObjInstance->ObjectPtr->vMeshes[mesh].vFaces[0].Material];
							if( matNumber > -1 )
							{
								newObjInstance->vMeshesMaterials.push_back(&newObjInstance->ObjectPtr->vMaterials[matNumber]);

								
								for( int mat = 0; mat < (int)CompleteMaterialsList.size();++mat)
								{
									if(CompleteMaterialsList[mat] == &newObjInstance->ObjectPtr->vMaterials[matNumber] )
									{
										newObjInstance->vMeshesMaterialsID.push_back(mat);
										materialFound = true;
										break;
									}
								}
								
								/*CompleteMaterialsList.push_back(&newObjInstance->ObjectPtr->vMaterials[matNumber]);
								int matID = (int)CompleteMaterialsList.size()-1;
								newObjInstance->vMeshesMaterialsID.push_back(matID);*/
							}
						}
					}
					if( materialFound == false)
							newObjInstance->vMeshesMaterialsID.push_back(0);
				} // mesh
				


			}
		}
	}

	for( int i = 0 ; i < Root->NbrCylinder ; ++i )
	{
		An8Mesh* cylinderMesh = NULL;
		cylinderMesh = AN8XMeshFromCylinder( cylinderMesh, &Root->vCylinders[i], AN8_NORMAL );
		if( cylinderMesh != NULL )
		{
			cylinderMesh->Triangulize();
			cylinderMesh->ComputePosition(); // Transform vertices from local space to object space
		}
		figureMeshesList->push_back(cylinderMesh); // mesh ptr
		figureMeshesListBoneIDs->push_back((int)(*boneList).size());// BoneID

		/* Matrix. We must:
			1) Rotate vertices with the current relative bone rotation matrix
			2) Transform the result with the parent matrix
		*/
		AN8XMATRIX boneRotationMatrix;
		AN8XMatrixRotationQuaternion( &boneRotationMatrix , &Root->Orientation );
		AN8XMATRIX matrix;
		AN8XMatrixMultiply(&matrix,i_ParentAbsoluteMatrix,&boneRotationMatrix);
		// Add Local translation
		AN8XMATRIX localTranslationMatrix;
		localTranslationMatrix._41 = cylinderMesh->BaseOrigin.x;
		localTranslationMatrix._42 = cylinderMesh->BaseOrigin.y;
		localTranslationMatrix._43 = cylinderMesh->BaseOrigin.z;
		AN8XMatrixMultiply(&matrix,&matrix,&localTranslationMatrix);
		// Add local rotation
		AN8XMATRIX localRotationMatrix;
		AN8XMatrixRotationQuaternion( &localRotationMatrix , &cylinderMesh->BaseOrientation );
		AN8XMatrixMultiply(&matrix,&matrix,&localRotationMatrix);
		figureMeshesListMatrices->push_back(matrix);

		// Materials
		bool materialFound = false;

		int matID = Root->vCylinders[i].MaterialID;
		if( i_File->vFigures[i_An8FigureID].NbrMaterial > 0 )
		{
			figureMeshesListMaterials->push_back(&i_File->vFigures[i_An8FigureID].vMaterials[matID]);

			for( int mat = 0; mat < (int)CompleteMaterialsList.size();++mat)
			{
				if(CompleteMaterialsList[mat] == &i_File->vFigures[i_An8FigureID].vMaterials[matID] )
				{
					MeshesListMaterialsID.push_back(mat);
					materialFound = true;
					break;
				}
			}
		}
		else
		{
			MeshesListMaterialsID.push_back(NULL);
		}
		if( materialFound == false)
			MeshesListMaterialsID.push_back(0);
		/*CompleteMaterialsList.push_back(&i_File->vFigures[i_An8FigureID].vMaterials[matID]);
		matID = (int)CompleteMaterialsList.size()-1; // material index in complete list of material
		MeshesListMaterialsID.push_back(matID);*/
	}

	//for( int i = 0 ; i < Root->NbrSphere ; ++i )
	for( int i = 0 ; i < Root->NbrSphere ; ++i )
	{
		An8Mesh* sphereMesh = NULL;
		sphereMesh = AN8XMeshFromSphere( sphereMesh, &Root->vSpheres[i], AN8_NORMAL );
		if( sphereMesh != NULL )
		{
			sphereMesh->Triangulize();
			sphereMesh->ComputePosition(); // Transform vertices from local space to object space
		}
		figureMeshesList->push_back(sphereMesh); // mesh ptr
		figureMeshesListBoneIDs->push_back((int)(*boneList).size());// BoneID

		/* Matrix. We must:
			1) Rotate vertices with the current relative bone rotation matrix
			2) Transform the result with the parent matrix
		*/
		AN8XMATRIX boneRotationMatrix;
		AN8XMatrixRotationQuaternion( &boneRotationMatrix , &Root->Orientation );
		AN8XMATRIX matrix;
		AN8XMatrixMultiply(&matrix,i_ParentAbsoluteMatrix,&boneRotationMatrix);
		// Add Local translation
		AN8XMATRIX localTranslationMatrix;
		localTranslationMatrix._41 = sphereMesh->BaseOrigin.x;
		localTranslationMatrix._42 = sphereMesh->BaseOrigin.y;
		localTranslationMatrix._43 = sphereMesh->BaseOrigin.z;
		AN8XMatrixMultiply(&matrix,&matrix,&localTranslationMatrix);
		// Add local rotation
		AN8XMATRIX localRotationMatrix;
		AN8XMatrixRotationQuaternion( &localRotationMatrix , &sphereMesh->BaseOrientation );
		AN8XMatrixMultiply(&matrix,&matrix,&localRotationMatrix);
		figureMeshesListMatrices->push_back(matrix);

		// Materials
		bool materialFound = false;
		int matID = Root->vSpheres[i].MaterialID;
		if( i_File->vFigures[i_An8FigureID].NbrMaterial > 0 )
		{
			figureMeshesListMaterials->push_back(&i_File->vFigures[i_An8FigureID].vMaterials[matID]);

			
			for( int mat = 0; mat < (int)CompleteMaterialsList.size();++mat)
			{
				if(CompleteMaterialsList[mat] == &i_File->vFigures[i_An8FigureID].vMaterials[matID] )
				{
					MeshesListMaterialsID.push_back(mat);
					materialFound = true;
					break;
				}
			}
		}
		else
		{
			MeshesListMaterialsID.push_back(NULL);
		}
		if( materialFound == false)
			MeshesListMaterialsID.push_back(0);
		/*CompleteMaterialsList.push_back(&i_File->vFigures[i_An8FigureID].vMaterials[matID]);
		matID = (int)CompleteMaterialsList.size()-1;
		MeshesListMaterialsID.push_back(matID);*/
	}


	boneList->push_back(newBone);

	for( int i = 0 ; i < (int) Root->vBones.size(); ++i)
	{
		ProcessNode(i_File,
			boneList,objectInstance,
			figureMeshesList,figureMeshesListBoneIDs,figureMeshesListMatrices,figureMeshesListMaterials,
			i_An8FigureID,&Root->vBones[i],
			&absoluteMatrix);
	}
}

/*

Les named object sont contenu dans les bones et contiennent tous les poids
d'une instance d'un objet.
Plusieurs named object peuvent concerné le même object.
Les id des bones contenu dans named object ne sont pas les id réel, il faut passé par les weightedBy des meshes.

Il faudrais rajouté un vector de namedObjects appelé objectsWeights contenant un pointeur vers l'object (An8) + 
les poids (définitif).

Pour les influence ? Si un mesh a déjà des paintedweight, on ignore les influences.

*/

bool AN8XFigureHelper::ComputeData(
          An8File* i_File, // le fichier an8 (objects, figures)
          int i_FigureID, // l'id de la figure
          std::vector<AN8XBone>* boneList, // List of bones
		  std::vector<AN8XObjectInstanceInFigure*>* objectInstance,
          std::vector<An8Mesh*>* figureMeshesList, // Must be deleted by you
          std::vector<int>* figureMeshesListBoneIDs,
          std::vector<AN8XMATRIX>* figureMeshesListMatrices,
		  std::vector<An8Material*>* figureMeshesListMaterials
)
{
	// Add materials
	for( int obj = 0; obj < (int)i_File->vObjects.size();++obj)
	{
		for( int mat = 0; mat < (int)i_File->vObjects[obj].vMaterials.size();++mat)
		{
			CompleteMaterialsList.push_back(&i_File->vObjects[obj].vMaterials[mat]);
		}
	}

	AN8XMATRIX absoluteMatrix; // Identity Matrix
	ProcessNode(i_File,
		boneList,objectInstance,
			figureMeshesList,figureMeshesListBoneIDs,figureMeshesListMatrices,figureMeshesListMaterials,
		i_FigureID,&i_File->vFigures[i_FigureID].Root,&absoluteMatrix);




	
	// Compute the final boneId for painted weights
	//i_pFigure->ComputeFinalPaintedWeightsBoneID();

	// Compute Parent
	for( int bone = 0; bone < (int)boneList->size(); ++bone)
	{
		// Compute Parent
		for( int p = 0; p < (int)boneList->size(); ++p)
		{
			std::string parentName = boneList->at(bone).ParentName;
			std::string name = boneList->at(p).Name;
			if( parentName.compare( name ) == 0 )
			{
				boneList->at(bone).Parent = &boneList->at(p);
				boneList->at(bone).ParentID = p;
			}
		}
	}

	// Compute Translation
	for( int bone = 1; bone < (int)boneList->size(); ++bone)
	{
		boneList->at(bone).Translation.z = boneList->at(bone).Parent->Length;
	}

	// All bone are loaded, we can compute weights
	for( int i = 0; i < (int)objectInstance->size();++i)
	{
		objectInstance->at(i)->ComputePaintedWeights(boneList);
		objectInstance->at(i)->ComputeInfluences(boneList);
	}

	return true;
}

bool AN8XFigureHelper::GetData(An8File* i_an8file,int figureID)
{
	if( ComputeData(
			i_an8file, // le fichier an8 (objects, figures)
			figureID, // l'id de la figure
			&vBones, // List of bones
			&ObjectInstance,
			&MeshesList, // Must be deleted by you
			&MeshesListBoneIDs,
			&MeshesListMatrices,
			&MeshesListMaterials
			) == false )
	{
			return false;
	}
	FigurePtr = &i_an8file->vFigures[figureID];
	return true;
}





