// This file in a part of LibAn8, Copyright (c) 2002-2007, Grégory Smialek
// It's licence can be found in an8loader.h included in this package
// WebSite: texel3d.free.fr
#include "An8Loader.h"
using namespace std;

An8Element::An8Element(void)
{
	Loc          = AN8XVECTOR3(0.0f,0.0f,0.0f);         // Location
	Orientation  = AN8XQUATERNION(0.0f,0.0f,0.0f,1.0f); // Orientation (quaternion)
	AN8XMatrixIdentity(&Matrix);	
	
	EnableRoll   = false;

	Scale = 1.0f;

	LookAtLoc = AN8XVECTOR3(0.0f,0.0f,0.0f); 
	LookAtEye = AN8XVECTOR3(0.0f,0.0f,0.0f);     
	LookAtUp  = AN8XVECTOR3(0.0f,0.0f,0.0f);     
	
	Locked = false;

	NbrAttribute = 0;
	NbrController = 0;

	RelativeTo = AN8_PARENT;

	NbrCamera = 0;
	NbrObjectElement = 0; 
	NbrFigureElement = 0;
	NbrNull  = 0;
	NbrLight = 0;    

	ptrParent = NULL;
	ptrTarget = NULL;
}

An8Element::~An8Element(void)
{
}

void An8Element::SetElementsPointer(An8Scene* i_Scene, An8Element* i_ptrParent)
{
	ptrParent = i_ptrParent;
	if( RelativeTo == AN8_OTHER || RelativeTo == AN8_FACINGOTHER )
		ptrTarget = i_Scene->GetTargetPointer(ptrTarget,TargetName);

	for( int i = 0 ; i < NbrObjectElement ; ++i )
		vObjectElements[i].SetElementsPointer(i_Scene,this);
	for( int i = 0 ; i < NbrFigureElement ; ++i )
		vFigureElements[i].SetElementsPointer(i_Scene,this);
	for( int i = 0 ; i < NbrCamera ; ++i )
		vCameras[i].SetElementsPointer(i_Scene,this);
	for( int i = 0 ; i < NbrLight ; ++i )
		vLights[i].SetElementsPointer(i_Scene,this);
	for( int i = 0 ; i < NbrNull ; ++i )
		vNulls[i].SetElementsPointer(i_Scene,this);
}

An8Element* An8Element::GetTargetPointer(An8Element* o_ptr, std::string i_TargetName)
{
	if( Name.compare(i_TargetName) == 0 )
	{
		o_ptr   = this;
		return o_ptr;
	}

	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		o_ptr = vObjectElements[i].GetTargetPointer(o_ptr,i_TargetName);
		if( o_ptr != NULL )
			return o_ptr;
	}
	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		o_ptr = vFigureElements[i].GetTargetPointer(o_ptr,i_TargetName);
		if( o_ptr != NULL )
			return o_ptr;
	}
	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		o_ptr = vCameras[i].GetTargetPointer(o_ptr,i_TargetName);
		if( o_ptr != NULL )
			return o_ptr;
	}
	for( int i = 0 ; i < NbrLight ; ++i )
	{
		o_ptr = vLights[i].GetTargetPointer(o_ptr,i_TargetName);
		if( o_ptr != NULL )
			return o_ptr;
	}
	for( int i = 0 ; i < NbrNull ; ++i )
	{
		o_ptr = vNulls[i].GetTargetPointer(o_ptr,i_TargetName);
		if( o_ptr != NULL )
			return o_ptr;
	}
	return NULL;
}

void An8Element::ComputeAllMatrices( void )
{
	this->ComputeMatrices();

	for( int i = 0 ; i < NbrObjectElement ; ++i )
		vObjectElements[i].ComputeAllMatrices();
	for( int i = 0 ; i < NbrFigureElement ; ++i )
		vFigureElements[i].ComputeAllMatrices();
	for( int i = 0 ; i < NbrCamera ; ++i )
		vCameras[i].ComputeAllMatrices();
	for( int i = 0 ; i < NbrLight ; ++i )
		vLights[i].ComputeAllMatrices();
	for( int i = 0 ; i < NbrNull ; ++i )
		vNulls[i].ComputeAllMatrices();
}


void An8Element::ComputeMatrices( void )
{
	// if( IsMatrixComputed ) o_Matrix = Matrix;return; 

	if( RelativeTo != AN8_FACINGOTHER && RelativeTo != AN8_OTHER )
	{
		ComputeLookAt(NULL); 
		ComputeSimpleMatrix();

		if( ptrParent != NULL)
		{
			ptrParent->ComputeMatrices();

			AN8XMATRIX parentMatrix; // parent matrix without scale
			AN8XMatrixMultiply(&parentMatrix,&ptrParent->OrientationMatrix,&ptrParent->TranslationMatrix);

			// Absolute matrices
			AN8XMatrixMultiply(&this->OrientationMatrix,&ptrParent->OrientationMatrix,&this->OrientationMatrix); 
			AN8XMatrixMultiply(&Matrix,&ptrParent->OrientationMatrix,&Matrix); 
			AN8XMatrixMultiply(&Matrix,&ptrParent->TranslationMatrix,&Matrix); 

		}
	}
	else if( RelativeTo == AN8_OTHER )
	{
		AN8XVECTOR3 targetLoc;
		if( ptrParent == NULL)
		{
			targetLoc = this->Loc;
		}
		else
		{
			ptrParent->ComputeMatrices();

			AN8XMATRIX parentMatrix; // parent matrix without scale
			AN8XMatrixMultiply(&parentMatrix,&ptrParent->TranslationMatrix,&ptrParent->OrientationMatrix);

			AN8XVECTOR4 tmpLoc;
			AN8XVec3Transform(&tmpLoc, &this->Loc, &parentMatrix );
			targetLoc.x = tmpLoc.x;
			targetLoc.y = tmpLoc.y;
			targetLoc.z = tmpLoc.z;
		}



		ptrTarget->ComputeMatrices();

		//AN8XVECTOR3 targetLoc = this->Loc;
		AN8XVECTOR3 targetEye =  targetLoc + (ptrTarget->LookAtEye - ptrTarget->LookAtLoc); 
		AN8XVECTOR3 targetUp = ptrTarget->LookAtUp;

		AN8XMATRIX matrixLookAt;
		AN8XMatrixLookAt(&matrixLookAt , &targetLoc, &targetEye , &targetUp); 

		// Compute local Lookat
		AN8XQUATERNION quat;
		quat.x = -Orientation.x;
		quat.y = -Orientation.y;
		quat.z = -Orientation.z;
		quat.w = Orientation.w;
		AN8XMATRIX matrix;
		AN8XMatrixRotationQuaternion( &matrix , &quat );
		AN8XVECTOR3 loc3;
		AN8XVECTOR4 eye4;
		AN8XVECTOR3 eye3;
		AN8XVECTOR4 up4;
		AN8XVECTOR3 up3;
		eye3.x = 0.0f;
		eye3.y = 0.0f;
		eye3.z = 1.0f;
		AN8XVec3Transform(&eye4, &eye3, &matrix );
		eye3.x = 0.0f + eye4.x;
		eye3.y = 0.0f + eye4.y;
		eye3.z = 0.0f + eye4.z;

		up3.x = 0.0f;
		up3.y = 1.0f;
		up3.z = 0.0f;

		if( EnableRoll == true )
		{
			AN8XVECTOR4 up;
			AN8XVec3Transform(&up, &up3, &matrix );
			up3.x = up.x;
			up3.y = up.y;
			up3.z = up.z;
		}
		AN8XVec3Normalize( &up3, &up3);
		loc3.x = 0.0f;loc3.y = 0.0f;loc3.z = 0.0f;
		AN8XMATRIX matrixLocalLookAt;
		AN8XMatrixLookAt(&matrixLocalLookAt , &loc3, &eye3 , &up3); 


		// Compute final lookat ///////////////////////////////////
	
		AN8XVECTOR4 vec4;
		AN8XVECTOR3 loc = AN8XVECTOR3(0.0f,0.0f,0.0f);
		AN8XVECTOR3 eye = AN8XVECTOR3(0.0f,0.0f,-1.0f);
		AN8XVECTOR3 up = AN8XVECTOR3(0.0f,1.0f,0.0f);

		AN8XMATRIX matrixInverseLookat;
		AN8XMatrixCopy( &matrixInverseLookat , &matrixLocalLookAt);

		AN8XVec3Transform(&vec4, &loc, &matrixInverseLookat ); 
		LookAtLoc.x = vec4.x;LookAtLoc.y = vec4.y;LookAtLoc.z = vec4.z;
		AN8XVec3Transform(&vec4, &eye, &matrixInverseLookat ); 
		LookAtEye.x = -vec4.x;LookAtEye.y = -vec4.y;LookAtEye.z = -vec4.z;
		//AN8XVec3TransformNormal(&vec4, &up, &matrixInverseLookat );
		//LookAtUp.x = vec4.x;LookAtUp.y = vec4.y;LookAtUp.z = vec4.z;
		AN8XVec3TransformNormal(&LookAtUp, &up, &matrixInverseLookat );

		AN8XMatrixInverse(&matrixInverseLookat,NULL,&matrixLookAt);

		AN8XVec3Transform(&vec4, &LookAtLoc, &matrixInverseLookat ); 
		LookAtLoc.x = vec4.x;LookAtLoc.y = vec4.y;LookAtLoc.z = vec4.z;
		AN8XVec3Transform(&vec4, &LookAtEye, &matrixInverseLookat ); 
		LookAtEye.x = vec4.x;LookAtEye.y = vec4.y;LookAtEye.z = vec4.z;
		//AN8XVec3TransformNormal(&vec4, &LookAtUp, &matrixInverseLookat );
		//LookAtUp.x = vec4.x;LookAtUp.y = vec4.y;LookAtUp.z = vec4.z;
		AN8XVec3TransformNormal(&LookAtUp, &LookAtUp, &matrixInverseLookat );

		if( EnableRoll == false ) 
		{
			LookAtUp.x = 0.0f;
			LookAtUp.y = 1.0f;
			LookAtUp.z = 0.0f;
		}

		// End
		AN8XVECTOR3 loctmp = AN8XVECTOR3(LookAtLoc.x,LookAtLoc.y,LookAtLoc.z);
		AN8XVECTOR3 eyetmp = LookAtLoc + (LookAtLoc - LookAtEye);
		AN8XVECTOR3 uptmp = AN8XVECTOR3(LookAtUp.x,LookAtUp.y,LookAtUp.z);
		
		AN8XMatrixLookAt(&Matrix , &loctmp, &eyetmp , &uptmp); 
		AN8XMatrixInverse(&Matrix,NULL,&Matrix);

		this->OrientationMatrix = this->Matrix;
		this->OrientationMatrix._41 = 0.0f;
		this->OrientationMatrix._42 = 0.0f;
		this->OrientationMatrix._43 = 0.0f;

		this->TranslationMatrix._41 = this->Matrix._41;
		this->TranslationMatrix._42 = this->Matrix._42;
		this->TranslationMatrix._43 = this->Matrix._43;

		AN8XMatrixScaling(&this->ScaleMatrix,Scale,Scale,Scale);
		AN8XMatrixMultiply(&Matrix,&Matrix,&this->ScaleMatrix);

	}
    else if( RelativeTo == AN8_FACINGOTHER )
	{
		if( ptrParent != NULL)
		{
			ptrTarget->ComputeMatrices();
			AN8XVECTOR4 targetAbsolutePosition = AN8XVECTOR4(ptrTarget->Matrix._41,ptrTarget->Matrix._42,ptrTarget->Matrix._43,1.0f);

			ptrParent->ComputeMatrices();
			AN8XVECTOR3 savedLoc;
			savedLoc.x = Loc.x;
			savedLoc.y = Loc.y;
			savedLoc.z = Loc.z;
			AN8XVECTOR4 absolutLoc;
			AN8XVec3Transform(&absolutLoc, &this->Loc, &ptrParent->Matrix ); 
			Loc.x = absolutLoc.x;
			Loc.y = absolutLoc.y;
			Loc.z = absolutLoc.z;
			ComputeLookAt(&targetAbsolutePosition);
			Loc.x = savedLoc.x;
			Loc.y = savedLoc.y;
			Loc.z = savedLoc.z;
			
			AN8XVECTOR3 loc = AN8XVECTOR3(LookAtLoc.x,LookAtLoc.y,LookAtLoc.z);
			AN8XVECTOR3 eye = LookAtLoc + (LookAtLoc - LookAtEye);
			AN8XVECTOR3 up = AN8XVECTOR3(LookAtUp.x,LookAtUp.y,LookAtUp.z);
			AN8XMatrixLookAt(&Matrix , &loc, &eye , &up); 
			AN8XMatrixInverse(&Matrix,NULL,&Matrix);

			this->OrientationMatrix = this->Matrix;
			this->OrientationMatrix._41 = 0.0f;
			this->OrientationMatrix._42 = 0.0f;
			this->OrientationMatrix._43 = 0.0f;

			this->TranslationMatrix._41 = this->Matrix._41;
			this->TranslationMatrix._42 = this->Matrix._42;
			this->TranslationMatrix._43 = this->Matrix._43;

			AN8XMatrixScaling(&this->ScaleMatrix,Scale,Scale,Scale);
			AN8XMatrixMultiply(&Matrix,&Matrix,&this->ScaleMatrix);
		}
		else
		{
			// Compute LookAt and Matrix using the absolute position of the target
			ptrTarget->ComputeMatrices();
			AN8XVECTOR4 targetAbsolutePosition = AN8XVECTOR4(ptrTarget->Matrix._41,ptrTarget->Matrix._42,ptrTarget->Matrix._43,1.0f);

			ComputeLookAt(&targetAbsolutePosition);

			AN8XVECTOR3 loc = AN8XVECTOR3(LookAtLoc.x,LookAtLoc.y,LookAtLoc.z);
			AN8XVECTOR3 eye = LookAtLoc + (LookAtLoc - LookAtEye);
			AN8XVECTOR3 up = AN8XVECTOR3(LookAtUp.x,LookAtUp.y,LookAtUp.z);
			AN8XMatrixLookAt(&Matrix , &loc, &eye , &up); 
			AN8XMatrixInverse(&Matrix,NULL,&Matrix);

			this->OrientationMatrix = this->Matrix;
			this->OrientationMatrix._41 = 0.0f;
			this->OrientationMatrix._42 = 0.0f;
			this->OrientationMatrix._43 = 0.0f;

			this->TranslationMatrix._41 = this->Matrix._41;
			this->TranslationMatrix._42 = this->Matrix._42;
			this->TranslationMatrix._43 = this->Matrix._43;

			AN8XMatrixScaling(&this->ScaleMatrix,Scale,Scale,Scale);
			AN8XMatrixMultiply(&Matrix,&Matrix,&this->ScaleMatrix);

		}
	}
	//else if( RelativeTo == AN8_PATH ){} // Not supported



}


//////////////////////////////////////////////////////////
//	Compute Matrix							       	    //
//////////////////////////////////////////////////////////
void An8Element::ComputeSimpleMatrix( void )
{
	if( EnableRoll == true )
	{
		AN8XMatrixRotationQuaternion( &OrientationMatrix , &Orientation );

		// Translation
		AN8XMatrixIdentity(&TranslationMatrix);
		TranslationMatrix._41 = Loc.x;
		TranslationMatrix._42 = Loc.y;
		TranslationMatrix._43 = Loc.z;

		AN8XMatrixMultiply(&Matrix,&TranslationMatrix,&OrientationMatrix);
	}
	else
	{
		AN8XVECTOR3 loc; // 0 0 0
		AN8XVECTOR3 eye =  LookAtEye - LookAtLoc; 
		AN8XVECTOR3 up = LookAtUp;
		AN8XMatrixLookAt(&OrientationMatrix , &LookAtLoc, &LookAtEye , &LookAtUp); 

		// Translation
		AN8XMatrixIdentity(&TranslationMatrix);
		TranslationMatrix._41 = LookAtLoc.x;
		TranslationMatrix._42 = LookAtLoc.y;
		TranslationMatrix._43 = LookAtLoc.z;

		AN8XMatrixMultiply(&Matrix,&TranslationMatrix,&OrientationMatrix);
	}
	// Scale
	AN8XMatrixScaling(&ScaleMatrix,Scale,Scale,Scale);
	AN8XMatrixMultiply(&Matrix,&Matrix,&ScaleMatrix);

}



//////////////////////////////////////////////////////////
//	Compute camera target								//
//////////////////////////////////////////////////////////
void An8Element::ComputeLookAt( AN8XVECTOR4* i_targetPosition)
{	
	if( i_targetPosition == NULL ) // If no target, lookat use element's matrix/position
	{
		AN8XMATRIX matrix;
		AN8XMatrixRotationQuaternion( &matrix , &Orientation );
		AN8XVECTOR4 eye;
		LookAtEye.x = 0.0f;
		LookAtEye.y = 0.0f;
		LookAtEye.z = 1.0f;
		AN8XVec3Transform(&eye, &LookAtEye, &matrix );
		LookAtEye.x = Loc.x + eye.x;
		LookAtEye.y = Loc.y + eye.y;
		LookAtEye.z = Loc.z + eye.z;

		LookAtUp.x = 0.0f;
		LookAtUp.y = 1.0f;
		LookAtUp.z = 0.0f;

		if( EnableRoll == true )
		{
			AN8XVECTOR4 up;
			AN8XVec3Transform(&up, &LookAtUp, &matrix );
			LookAtUp.x = up.x;
			LookAtUp.y = up.y;
			LookAtUp.z = up.z;
		}
		AN8XVec3Normalize( &LookAtUp, &LookAtUp);
		LookAtLoc = Loc;
	}
	else // If target, use its absolute position
	{
		LookAtLoc = Loc;
		LookAtEye.x = i_targetPosition->x;
		LookAtEye.y = i_targetPosition->y;
		LookAtEye.z = i_targetPosition->z;

		LookAtUp.x = 0.0f;
		LookAtUp.y = 1.0f;
		LookAtUp.z = 0.0f;

		// Multiply lookat by relative orientation/loc
		AN8XMATRIX matrixLookAt;
		AN8XMatrixLookAt(&matrixLookAt , &LookAtLoc, &LookAtEye , &LookAtUp); 


		// Compute local Lookat
		AN8XQUATERNION quat;
		quat.x = -Orientation.x;
		quat.y = -Orientation.y;
		quat.z = -Orientation.z;
		quat.w = Orientation.w;
		AN8XMATRIX matrix;
		AN8XMatrixRotationQuaternion( &matrix , &quat );
		AN8XVECTOR3 loc3;
		AN8XVECTOR4 eye4;
		AN8XVECTOR3 eye3;
		AN8XVECTOR4 up4;
		AN8XVECTOR3 up3;
		eye3.x = 0.0f;
		eye3.y = 0.0f;
		eye3.z = 1.0f;
		AN8XVec3Transform(&eye4, &eye3, &matrix );
		eye3.x = 0.0f + eye4.x;
		eye3.y = 0.0f + eye4.y;
		eye3.z = 0.0f + eye4.z;

		up3.x = 0.0f;
		up3.y = 1.0f;
		up3.z = 0.0f;

		if( EnableRoll == true )
		{
			AN8XVECTOR4 up;
			AN8XVec3Transform(&up, &up3, &matrix );
			up3.x = up.x;
			up3.y = up.y;
			up3.z = up.z;
		}
		AN8XVec3Normalize( &up3, &up3);
		loc3.x = 0.0f;loc3.y = 0.0f;loc3.z = 0.0f;
		AN8XMATRIX matrixLocalLookAt;
		AN8XMatrixLookAt(&matrixLocalLookAt , &loc3, &eye3 , &up3); 


		// Compute final lookat ///////////////////////////////////
	
		AN8XVECTOR4 vec4;
		AN8XVECTOR3 loc = AN8XVECTOR3(0.0f,0.0f,0.0f);
		AN8XVECTOR3 eye = AN8XVECTOR3(0.0f,0.0f,-1.0f);
		AN8XVECTOR3 up = AN8XVECTOR3(0.0f,1.0f,0.0f);

		AN8XMATRIX matrixInverseLookat;
		AN8XMatrixCopy( &matrixInverseLookat , &matrixLocalLookAt);

		AN8XVec3Transform(&vec4, &loc, &matrixInverseLookat ); 
		LookAtLoc.x = vec4.x;LookAtLoc.y = vec4.y;LookAtLoc.z = vec4.z;
		AN8XVec3Transform(&vec4, &eye, &matrixInverseLookat ); 
		LookAtEye.x = -vec4.x;LookAtEye.y = -vec4.y;LookAtEye.z = -vec4.z;
		//AN8XVec3TransformNormal(&vec4, &up, &matrixInverseLookat );
		//LookAtUp.x = vec4.x;LookAtUp.y = vec4.y;LookAtUp.z = vec4.z;
		AN8XVec3TransformNormal(&LookAtUp, &up, &matrixInverseLookat );



		AN8XMatrixInverse(&matrixInverseLookat,NULL,&matrixLookAt);

		AN8XVec3Transform(&vec4, &LookAtLoc, &matrixInverseLookat ); 
		LookAtLoc.x = vec4.x;LookAtLoc.y = vec4.y;LookAtLoc.z = vec4.z;
		AN8XVec3Transform(&vec4, &LookAtEye, &matrixInverseLookat ); 
		LookAtEye.x = vec4.x;LookAtEye.y = vec4.y;LookAtEye.z = vec4.z;
		//AN8XVec3TransformNormal(&vec4, &LookAtUp, &matrixInverseLookat );
		//LookAtUp.x = vec4.x;LookAtUp.y = vec4.y;LookAtUp.z = vec4.z;
		AN8XVec3TransformNormal(&LookAtUp, &LookAtUp, &matrixInverseLookat );

	}
}

std::vector<An8ObjectElement*> An8Element::GetObjectElements(void)
{
	std::vector<An8ObjectElement*> objectsToReturn;
	
	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		objectsToReturn.push_back(&vObjectElements[i]);
		std::vector<An8ObjectElement*> objects = vObjectElements[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vFigureElements[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vCameras[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vLights[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8ObjectElement*> objects = vNulls[i].GetObjectElements();
		for(int j = 0 ; j< (int)objects.size() ; ++j )
			objectsToReturn.push_back(objects[j]);
	}

	return objectsToReturn;
}


std::vector<An8Light*> An8Element::GetLights(void)
{
	std::vector<An8Light*> lightsToReturn;
	
	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8Light*> lights = vObjectElements[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8Light*> lights = vFigureElements[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		std::vector<An8Light*> lights = vCameras[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		lightsToReturn.push_back(&vLights[i]);
		std::vector<An8Light*> lights = vLights[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8Light*> lights = vNulls[i].GetLights();
		for(int j = 0 ; j< (int)lights.size() ; ++j )
			lightsToReturn.push_back(lights[j]);
	}

	return lightsToReturn;
}




std::vector<An8FigureElement*> An8Element::GetFigureElements(void)
{
	std::vector<An8FigureElement*> figuresToReturn;
	
	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8FigureElement*> figures = vObjectElements[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		figuresToReturn.push_back(&vFigureElements[i]);
		std::vector<An8FigureElement*> figures = vFigureElements[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		std::vector<An8FigureElement*> figures = vCameras[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8FigureElement*> figures = vLights[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8FigureElement*> figures = vNulls[i].GetFigureElements();
		for(int j = 0 ; j< (int)figures.size() ; ++j )
			figuresToReturn.push_back(figures[j]);
	}

	return figuresToReturn;
}
std::vector<An8Camera*> An8Element::GetCameras(void)
{
	std::vector<An8Camera*> camerasToReturn;
	
	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8Camera*> cameras = vObjectElements[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8Camera*> cameras = vFigureElements[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		camerasToReturn.push_back(&vCameras[i]);
		std::vector<An8Camera*> cameras = vCameras[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8Camera*> cameras = vLights[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		std::vector<An8Camera*> cameras = vNulls[i].GetCameras();
		for(int j = 0 ; j< (int)cameras.size() ; ++j )
			camerasToReturn.push_back(cameras[j]);
	}

	return camerasToReturn;
}
std::vector<An8Null*> An8Element::GetNulls(void)
{
	std::vector<An8Null*> nullsToReturn;
	
	for( int i = 0 ; i < NbrObjectElement ; ++i )
	{
		std::vector<An8Null*> nulls = vObjectElements[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrFigureElement ; ++i )
	{
		std::vector<An8Null*> nulls = vFigureElements[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrCamera ; ++i )
	{
		
		std::vector<An8Null*> nulls = vCameras[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrLight ; ++i )
	{
		std::vector<An8Null*> nulls = vLights[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	for( int i = 0 ; i < NbrNull ; ++i )
	{
		nullsToReturn.push_back(&vNulls[i]);
		std::vector<An8Null*> nulls = vNulls[i].GetNulls();
		for(int j = 0 ; j< (int)nulls.size() ; ++j )
			nullsToReturn.push_back(nulls[j]);
	}

	return nullsToReturn;
}

