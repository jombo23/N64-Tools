/*
* AN8X.h and AN8X.cpp are not under the same licence as Liban8
* AN8X.h and AN8X.cpp use some source code of GLGX
* See the copyright notice of the distribution
* The licence below is the licence of AN8X.h and AN8X.cpp:
*
* Copyright (c) 2002-2008, Grégory Smialek
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted
* provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice, this list of conditions
*    and the following disclaimer in the documentation and/or other materials provided with the distribution.
*  * Redistributions in binary form must reproduce the above copyright notice, this list of
*    conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
*  * Neither the name of the Grégory Smialek nor the names of its contributors may be used
*    to endorse or promote products derived from this software without specific prior written
*    permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include"AN8X.h"
#include<math.h>



AN8XVECTOR4 AN8XVECTOR4::operator+ (const AN8XVECTOR4 &B)
{
	return AN8XVECTOR4( this->x + B.x, this->y + B.y, this->z + B.z, this->w + B.w);
}

AN8XVECTOR4 AN8XVECTOR4::operator- (const AN8XVECTOR4 &B)
{
	return AN8XVECTOR4( this->x - B.x, this->y - B.y, this->z - B.z, this->w - B.w);
}


float AN8XVECTOR4::operator* (const AN8XVECTOR4 &B)
{
	return ( this->x * B.x ) + ( this->y * B.y ) + ( this->z * B.z ) + ( this->w * B.w );
}

AN8XVECTOR4 AN8XVECTOR4::operator* (float factor)
{
	return AN8XVECTOR4( this->x * factor, this->y * factor, this->z * factor, this->w * factor);
}

AN8XVECTOR4 AN8XVECTOR4::operator/ (float factor)
{
	return AN8XVECTOR4( this->x / factor, this->y / factor, this->z / factor, this->w / factor);
}


/////////////////////////////////////////////////////////////

AN8XVECTOR2 AN8XVECTOR2::operator+ (const AN8XVECTOR2 &B)
{
	return AN8XVECTOR2(this->x+B.x,this->y+B.y);
}

AN8XVECTOR2 AN8XVECTOR2::operator- (const AN8XVECTOR2 &B)
{
	return AN8XVECTOR2(this->x-B.x,this-> y-B.y);
}

float AN8XVECTOR2::operator* (const AN8XVECTOR2 &B)
{
	return ( this->x * B.x ) + ( this->y * B.y );
}

AN8XVECTOR2 AN8XVECTOR2::operator* (float factor)
{
	return AN8XVECTOR2(this->x*factor,this->y*factor);
}

AN8XVECTOR2 AN8XVECTOR2::operator/ (float factor)
{
	return AN8XVECTOR2(this->x/factor,this->y/factor);
}

AN8XVECTOR4* AN8XVec2Transform(AN8XVECTOR4 * dest,const AN8XVECTOR2 * src,const AN8XMATRIX * matrix)
{
	AN8XVECTOR4 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21  + matrix->_41;
	result.y = src->x * matrix->_12 + src->y * matrix->_22  + matrix->_42;
	result.z = src->x * matrix->_13 + src->y * matrix->_23  + matrix->_43;
	result.w = src->x * matrix->_14 + src->y * matrix->_24  + matrix->_44;

	dest->x = result.x;
	dest->y = result.y;
	dest->z = result.z;
	dest->w = result.w;
	return dest;
}

AN8XVECTOR2 * AN8XVec2TransformNormal(AN8XVECTOR2 * dest,const AN8XVECTOR2 * src,const AN8XMATRIX * matrix)
{
	AN8XVECTOR2 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21 ;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 ;

	dest->x = result.x;
	dest->y = result.y;
	return dest;
}

/////////////////////////////////////////////////////////////



AN8XVECTOR3 AN8XVECTOR3::operator+ (const AN8XVECTOR3 &B)
{
	return AN8XVECTOR3( this->x + B.x, this->y + B.y, this->z + B.z);
}

AN8XVECTOR3 AN8XVECTOR3::operator- (const AN8XVECTOR3 &B)
{
	return AN8XVECTOR3( this->x - B.x, this->y - B.y, this->z - B.z);
}

float AN8XVECTOR3::operator* (const AN8XVECTOR3 &B)
{
	return ( this->x * B.x ) + ( this->y * B.y ) + ( this->z * B.z );
}

AN8XVECTOR3 AN8XVECTOR3::operator* (float factor)
{
	return AN8XVECTOR3( this->x * factor, this->y * factor, this->z * factor);
}

AN8XVECTOR3 AN8XVECTOR3::operator/ (float factor)
{
	return AN8XVECTOR3( this->x / factor, this->y / factor, this->z / factor);
}


/////////////////////////////////////////////////////////////

AN8XVECTOR3* AN8XVec3Cross(AN8XVECTOR3* result,AN8XVECTOR3* A,AN8XVECTOR3* B)
{
	float x = A->y*B->z - A->z*B->y;
	float y = A->z*B->x - A->x*B->z;
	float z = A->x*B->y - A->y*B->x;
	result->x = x;
	result->y = y;
	result->z = z;

	return result;
}

float AN8XVec3Dot(const AN8XVECTOR3 *srcA,const AN8XVECTOR3 *srcB)
{
	return (srcA->x*srcB->x) + (srcA->y*srcB->y) + (srcA->z*srcB->z);
}

AN8XVECTOR3 *AN8XVec3Normalize(AN8XVECTOR3 *pOut,const AN8XVECTOR3 *pV)
{
	float magnitude =(float) sqrt( (pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z) );

	if( magnitude > 0.00001 )
	{
		pOut->x = pV->x / magnitude;
		pOut->y = pV->y / magnitude;
		pOut->z = pV->z / magnitude;
	}

	return pOut;
}

float AN8XVec3Length(const AN8XVECTOR3 *vector)
{
	return sqrt( vector->x*vector->x + vector->y*vector->y + vector->z*vector->z);
}

float AN8XVec2Length(const AN8XVECTOR2 *vector)
{
	return sqrt( vector->x*vector->x + vector->y*vector->y);
}



void AN8XVec3Lerp(AN8XVECTOR3 *dest,AN8XVECTOR3 *v1,AN8XVECTOR3 *v2,float l)
{
	AN8XVECTOR3 v = *v2 - *v1;

	*dest = *v1 + v*l;
}

AN8XVECTOR3 *AN8XVec3Scale(AN8XVECTOR3 *out,AN8XVECTOR3 *vector,float s)
{
	out->x = vector->x / s;
	out->y = vector->y / s;
	out->z = vector->z / s;

	return out;
}




AN8XVECTOR4 * AN8XVec3Transform(AN8XVECTOR4 * dest,const AN8XVECTOR3 * src,const AN8XMATRIX * matrix)
{
	AN8XVECTOR4 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21 + src->z * matrix->_31 + matrix->_41;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + src->z * matrix->_32 + matrix->_42;
	result.z = src->x * matrix->_13 + src->y * matrix->_23 + src->z * matrix->_33 + matrix->_43;
	result.w = src->x * matrix->_14 + src->y * matrix->_24 + src->z * matrix->_34 + matrix->_44;

	dest->x = result.x;
	dest->y = result.y;
	dest->z = result.z;
	dest->w = result.w;
	return dest;
}



AN8XVECTOR3 * AN8XVec3TransformCoord(AN8XVECTOR3 * dest,const AN8XVECTOR3 * src,const AN8XMATRIX * matrix)
{
	AN8XVECTOR3 result;

	float w = src->x * matrix->_14 + src->y * matrix->_24 + src->z * matrix->_34 + matrix->_44;
	result.x = src->x * matrix->_11 + src->y * matrix->_21 + src->z * matrix->_31 + matrix->_41;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + src->z * matrix->_32 + matrix->_42;
	result.z = src->x * matrix->_13 + src->y * matrix->_23 + src->z * matrix->_33 + matrix->_43;

	if( w != 0.0f)
	{
		dest->x = result.x / w;
		dest->y = result.y / w;
		dest->z = result.z / w;
	}
	else
	{
		dest->x = 0.0f;
		dest->y = 0.0f;
		dest->z = 0.0f;
	}
	return dest;
}


AN8XVECTOR4 * AN8XVec4Normalize(AN8XVECTOR4 *pOut,const AN8XVECTOR4 *pV)
{
	float magnitude =(float) sqrt( (pV->x * pV->x) + (pV->y * pV->y) + (pV->z * pV->z) + (pV->w * pV->w) );

	pOut->x = pV->x / magnitude;
	pOut->y = pV->y / magnitude;
	pOut->z = pV->z / magnitude;
	pOut->w = pV->w / magnitude;

	return pOut;
}


AN8XVECTOR3 * AN8XVec3TransformNormal(AN8XVECTOR3 * dest,const AN8XVECTOR3 * src,const AN8XMATRIX * matrix)
{
	AN8XVECTOR3 result;

	result.x = src->x * matrix->_11 + src->y * matrix->_21 + src->z * matrix->_31;
	result.y = src->x * matrix->_12 + src->y * matrix->_22 + src->z * matrix->_32;
	result.z = src->x * matrix->_13 + src->y * matrix->_23 + src->z * matrix->_33;
	//result.w = src->x * matrix->_14 + src->y * matrix->_24 + src->z * matrix->_34;

	dest->x = result.x;
	dest->y = result.y;
	dest->z = result.z;
	//dest->w = result.w;
	return dest;
}


AN8XVECTOR3 * AN8XVec3BaryCentric(AN8XVECTOR3 *o_Result,AN8XVECTOR3 *i_src1,AN8XVECTOR3 *i_src2,AN8XVECTOR3 *i_src3,float i_f,float i_g)
{
	*o_Result = *i_src1 + (*i_src2 - *i_src1)*i_f + (*i_src3 - *i_src1)*i_g;
	return o_Result;
}

AN8XVECTOR3 * AN8XVec3CatmullRom(AN8XVECTOR3 *o_Result,AN8XVECTOR3 *i_Src0,AN8XVECTOR3 *i_Src1,AN8XVECTOR3 *i_Src2,AN8XVECTOR3 *i_Src3,float i_t)
{
	float a = -(i_t*i_t*i_t) + 2*(i_t*i_t) - i_t;
	float b = 3*i_t*i_t*i_t - 5*(i_t*i_t) + 2;
	float c = -3*(i_t*i_t*i_t) + 4*(i_t*i_t) + i_t;
	float d = (i_t*i_t*i_t) - (i_t*i_t);

	o_Result->x = 0.5f * (a*i_Src0->x + b*i_Src1->x + c*i_Src2->x + d*i_Src3->x);
	o_Result->y = 0.5f * (a*i_Src0->y + b*i_Src1->y + c*i_Src2->y + d*i_Src3->y);
	o_Result->z = 0.5f * (a*i_Src0->z + b*i_Src1->z + c*i_Src2->z + d*i_Src3->z);
	return o_Result;
}

AN8XVECTOR3* AN8XVec3Hermite(AN8XVECTOR3 *o_Result, AN8XVECTOR3 *i_Pos0, AN8XVECTOR3 *i_Tan0, AN8XVECTOR3 *i_Pos1, AN8XVECTOR3 *i_Tan1, float i_t)
{
	float a = 2.0f*(i_t*i_t*i_t) - 3.0f*(i_t*i_t) + 1.0f;
	float b = (i_t*i_t*i_t) - 2.0f * (i_t*i_t) + i_t;
	float c = -2.0f *(i_t*i_t*i_t) + 3.0f*(i_t*i_t);
	float d = (i_t*i_t*i_t) - (i_t*i_t);

	o_Result->x = a * (i_Pos0->x) + b * (i_Tan0->x) + c * (i_Pos1->x) + d * (i_Tan1->x);
	o_Result->y = a * (i_Pos0->y) + b * (i_Tan0->y) + c * (i_Pos1->y) + d * (i_Tan1->y);
	o_Result->z = a * (i_Pos0->z) + b * (i_Tan0->z) + c * (i_Pos1->z) + d * (i_Tan1->z);

	return o_Result;
}

AN8XVECTOR3* AN8XVec3BezierDeg2(AN8XVECTOR3 *o_Result, AN8XVECTOR3 *i_P0, AN8XVECTOR3 *i_P1, AN8XVECTOR3 *i_P2, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t );
	float b = 2.0f*i_t*( 1.0f - i_t );
	float c = (i_t*i_t);

	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y);
	o_Result->z = a * (i_P0->z) + b * (i_P1->z) + c * (i_P2->z);

	return o_Result;
}

AN8XVECTOR3* AN8XVec3BezierDeg3(AN8XVECTOR3 *o_Result, AN8XVECTOR3 *i_P0, AN8XVECTOR3 *i_P1, AN8XVECTOR3 *i_P2, AN8XVECTOR3 *i_P3, float i_t)
{
	float a = ( 1.0f - i_t )*( 1.0f - i_t )*( 1.0f - i_t );
	float b = 3.0f*i_t*( 1.0f - i_t )*( 1.0f - i_t );
	float c = 3.0f*i_t*i_t*( 1.0f - i_t );
	float d = (i_t*i_t*i_t);

	o_Result->x = a * (i_P0->x) + b * (i_P1->x) + c * (i_P2->x) + d * (i_P3->x);
	o_Result->y = a * (i_P0->y) + b * (i_P1->y) + c * (i_P2->y) + d * (i_P3->y);
	o_Result->z = a * (i_P0->z) + b * (i_P1->z) + c * (i_P2->z) + d * (i_P3->z);

	return o_Result;
}

AN8XVECTOR2 *AN8XVec2Normalize(AN8XVECTOR2 *pOut,const AN8XVECTOR2 *pV)
{
	float magnitude =(float) sqrt( (pV->x * pV->x) + (pV->y * pV->y) );

	if( magnitude > 0.00001 )
	{
		pOut->x = pV->x / magnitude;
		pOut->y = pV->y / magnitude;
	}

	return pOut;
}




/////////////////////////////////////////////////////////////

void AN8XMatrixIdentity(AN8XMATRIX* matrix)
{
		matrix->_12 = matrix->_13 = matrix->_14 = matrix->_21 = matrix->_23 = matrix->_24 = matrix->_31 =matrix->_32 = matrix->_34 = matrix->_41 = matrix->_42 = matrix->_43 = 0.0f;
		matrix->_11 = matrix->_22 = matrix->_33 = matrix->_44 = 1.0f;
}

void AN8XMatrixMultiply( AN8XMATRIX* Result, AN8XMATRIX* A, AN8XMATRIX* B )
{
	AN8XMATRIX temp;

	temp._11 = A->_11*B->_11 + A->_21*B->_12 +  A->_31*B->_13 + A->_41*B->_14;
	temp._12 = A->_12*B->_11 + A->_22*B->_12 +  A->_32*B->_13 + A->_42*B->_14;
	temp._13 = A->_13*B->_11 + A->_23*B->_12 +  A->_33*B->_13 + A->_43*B->_14;
	temp._14 = A->_14*B->_11 + A->_24*B->_12 +  A->_34*B->_13 + A->_44*B->_14;

	temp._21 = A->_11*B->_21 + A->_21*B->_22 +  A->_31*B->_23 + A->_41*B->_24;
	temp._22 = A->_12*B->_21 + A->_22*B->_22 +  A->_32*B->_23 + A->_42*B->_24;
	temp._23 = A->_13*B->_21 + A->_23*B->_22 +  A->_33*B->_23 + A->_43*B->_24;
	temp._24 = A->_14*B->_21 + A->_24*B->_22 +  A->_34*B->_23 + A->_44*B->_24;

	temp._31  = A->_11*B->_31 + A->_21*B->_32 +  A->_31*B->_33 + A->_41*B->_34;
	temp._32  = A->_12*B->_31 + A->_22*B->_32 +  A->_32*B->_33 + A->_42*B->_34;
	temp._33 = A->_13*B->_31 + A->_23*B->_32 + A->_33*B->_33 + A->_43*B->_34;
	temp._34 = A->_14*B->_31 + A->_24*B->_32 + A->_34*B->_33 + A->_44*B->_34;

	temp._41 = A->_11*B->_41 + A->_21*B->_42 +  A->_31*B->_43 + A->_41*B->_44;
	temp._42 = A->_12*B->_41 + A->_22*B->_42 +  A->_32*B->_43 + A->_42*B->_44;
	temp._43 = A->_13*B->_41 + A->_23*B->_42 + A->_33*B->_43 + A->_43*B->_44;
	temp._44 = A->_14*B->_41 + A->_24*B->_42 + A->_34*B->_43 + A->_44*B->_44;

	float* dest = &Result->_11;
	float* src  = &temp._11;
	for( int i = 0; i < 16 ; ++i)
	{
		dest[i] = src[i];
	}
}

void AN8XMatrixTranspose( AN8XMATRIX* Result, AN8XMATRIX* matrix)
{
	AN8XMATRIX temp;

	temp._11 = matrix->_11;  temp._21 = matrix->_12;
	temp._12 = matrix->_21;  temp._22 = matrix->_22;
	temp._13 = matrix->_31;  temp._23 = matrix->_32;
	temp._14 = matrix->_41; temp._24 = matrix->_42;

	temp._31 = matrix->_13;  temp._41 = matrix->_14;
	temp._32 = matrix->_23;  temp._42 = matrix->_24;
	temp._33= matrix->_33; temp._43 = matrix->_34;
	temp._34= matrix->_43; temp._44 = matrix->_44;

	float* dest = &Result->_11;
	float* src  = &temp._11;
	for( int i = 0; i < 16 ; ++i)
	{
		dest[i] = src[i];
	}
}

void AN8XMatrixCopy( AN8XMATRIX* Result, AN8XMATRIX* matrix)
{
	float* dest = &Result->_11;
	float* src  = &matrix->_11;
	for( int i = 0; i < 16 ; ++i)
	{
		dest[i] = src[i];
	}
}


AN8XMATRIX* AN8XMatrixLookAt( AN8XMATRIX* pResult,AN8XVECTOR3 *pPosition,AN8XVECTOR3 *pTarget,AN8XVECTOR3 *pUp)
{

	AN8XMATRIX orient;

	AN8XVECTOR3 forward = *pPosition - *pTarget; // OK
	AN8XVec3Normalize(&forward,&forward);

	AN8XVECTOR3  up;
	AN8XVec3Normalize(&up,pUp);
	AN8XVECTOR3 side;
	AN8XVec3Cross(&side, &up, &forward);
	AN8XVec3Normalize(&side,&side);

	AN8XVec3Cross(&up, &forward, &side);
	AN8XVec3Normalize(&up,&up);

	orient._11 = side.x;
	orient._12 = side.y;
	orient._13 = side.z;
	orient._14 = 0.0f;

	orient._21 = up.x;
	orient._22 = up.y;
	orient._23 = up.z;
	orient._24 = 0.0f;

	orient._31 = forward.x;
	orient._32 = forward.y;
	orient._33 = forward.z;
	orient._34 = 0.0f;

	orient._41 =  0.0f;
	orient._42 =  0.0f;
	orient._43 =  0.0f;
	orient._44 =  1.0f;

	AN8XMatrixTranspose(&orient,&orient);

	AN8XMATRIX correction;

	correction._41 = -pPosition->x;
	correction._42 = -pPosition->y;
	correction._43 = -pPosition->z;
	correction._44 = 1.0f;

	AN8XMatrixMultiply(pResult,&orient,&correction);

	return pResult;
}


void AN8XMatrixRotationQuaternion( AN8XMATRIX* matrix, AN8XQUATERNION* quat )
{
	// Quaternion -> matrix
	matrix->_11= 1-(2*quat->y*quat->y)
						-(2*quat->z*quat->z);
	matrix->_21= (2*quat->x*quat->y)
						-(2*quat->w*quat->z);
	matrix->_31= (2*quat->x*quat->z)
						+(2*quat->w*quat->y);
	matrix->_41= 0.0f;

	matrix->_12= (2*quat->x*quat->y)
						+(2*quat->w*quat->z);
	matrix->_22= 1-(2*quat->x*quat->x)
						-(2*quat->z*quat->z);
	matrix->_32= (2*quat->y*quat->z)
						-(2*quat->w*quat->x);
	matrix->_42= 0.0f;

	matrix->_13= (2*quat->x*quat->z)
						-(2*quat->w*quat->y);
	matrix->_23= (2*quat->y*quat->z)
						+(2*quat->w*quat->x);
	matrix->_33= 1-(2*quat->x*quat->x)
						-(2*quat->y*quat->y);
	matrix->_43= 0.0f;

	matrix->_14= 0.0f;
	matrix->_24= 0.0f;
	matrix->_34= 0.0f;
	matrix->_44= 1.0f;
}


void AN8XMatrixTranslation(AN8XMATRIX* dest,float x, float y, float z)
{
	dest->_11  = 1.0f; dest->_12  = 0.0f; dest->_13  = 0.0f; dest->_14  = 0.0f;
	dest->_21  = 0.0f; dest->_22  = 1.0f; dest->_23  = 0.0f; dest->_24  = 0.0f;
	dest->_31  = 0.0f; dest->_32  = 0.0f; dest->_33 = 1.0f; dest->_34 = 0.0f;
	dest->_41 = x;    dest->_42 = y;    dest->_43 = z; dest->_44 = 1.0f;
}


void AN8XMatrixTranslation(AN8XMATRIX* dest,AN8XVECTOR3* vector)
{
	dest->_11  = 1.0f; dest->_12  = 0.0f; dest->_13  = 0.0f; dest->_14  = 0.0f;
	dest->_21  = 0.0f; dest->_22  = 1.0f; dest->_23  = 0.0f; dest->_24  = 0.0f;
	dest->_31  = 0.0f; dest->_32  = 0.0f; dest->_33 = 1.0f; dest->_34 = 0.0f;
	dest->_41 = vector->x; dest->_42 = vector->y; dest->_43 = vector->z; dest->_44 = 1.0f;
}

void AN8XMatrixScaling(AN8XMATRIX* dest,float x,float y,float z)
{
	dest->_11  = x; dest->_12  = 0.0f; dest->_13  = 0.0f; dest->_14  = 0.0f;
	dest->_21  = 0.0f; dest->_22  = y; dest->_23  = 0.0f; dest->_24  = 0.0f;
	dest->_31  = 0.0f; dest->_32  = 0.0f; dest->_33 = z; dest->_34 = 0.0f;
	dest->_41 = 0.0f; dest->_42 = 0.0f; dest->_43 = 0.0f; dest->_44 = 1.0f;
}

void AN8XMatrixScaling(AN8XMATRIX* dest,AN8XVECTOR3* vector)
{
	dest->_11  = vector->x; dest->_12  = 0.0f; dest->_13  = 0.0f; dest->_14  = 0.0f;
	dest->_21  = 0.0f; dest->_22  = vector->y; dest->_23  = 0.0f; dest->_24  = 0.0f;
	dest->_31  = 0.0f; dest->_32  = 0.0f; dest->_33 = vector->z; dest->_34 = 0.0f;
	dest->_41 = 0.0f; dest->_42 = 0.0f; dest->_43 = 0.0f; dest->_44 = 1.0f;
}



float AN8XAngleBetweenVec3(AN8XVECTOR3 *srcA,AN8XVECTOR3 *srcB)
{
	AN8XVECTOR3 vecA;
	AN8XVec3Normalize(&vecA,srcA);
	AN8XVECTOR3 vecB;
	AN8XVec3Normalize(&vecB,srcB);

	float cosAngle = AN8XVec3Dot(&vecA,&vecB);
	return acosf(cosAngle);
}






float AN8XMatrixDeterminant( AN8XMATRIX* i_matrix)
{
	return
   i_matrix->_41 * i_matrix->_32 * i_matrix->_23 * i_matrix->_14 - i_matrix->_31 * i_matrix->_42 * i_matrix->_23 * i_matrix->_14 - i_matrix->_41 * i_matrix->_22 * i_matrix->_33 * i_matrix->_14 + i_matrix->_21 * i_matrix->_42 * i_matrix->_33 * i_matrix->_14 +
   i_matrix->_31 * i_matrix->_22 * i_matrix->_43 * i_matrix->_14 - i_matrix->_21 * i_matrix->_32 * i_matrix->_43 * i_matrix->_14 - i_matrix->_41 * i_matrix->_32 * i_matrix->_13 * i_matrix->_24 + i_matrix->_31 * i_matrix->_42 * i_matrix->_13 * i_matrix->_24 +
   i_matrix->_41 * i_matrix->_12 * i_matrix->_33 * i_matrix->_24 - i_matrix->_11 * i_matrix->_42 * i_matrix->_33 * i_matrix->_24 - i_matrix->_31 * i_matrix->_12 * i_matrix->_43 * i_matrix->_24 + i_matrix->_11 * i_matrix->_32 * i_matrix->_43 * i_matrix->_24 +
   i_matrix->_41 * i_matrix->_22 * i_matrix->_13 * i_matrix->_34 - i_matrix->_21 * i_matrix->_42 * i_matrix->_13 * i_matrix->_34 - i_matrix->_41 * i_matrix->_12 * i_matrix->_23 * i_matrix->_34 + i_matrix->_11 * i_matrix->_42 * i_matrix->_23 * i_matrix->_34 +
   i_matrix->_21 * i_matrix->_12 * i_matrix->_43 * i_matrix->_34 - i_matrix->_11 * i_matrix->_22 * i_matrix->_43 * i_matrix->_34 - i_matrix->_31 * i_matrix->_22 * i_matrix->_13 * i_matrix->_44 + i_matrix->_21 * i_matrix->_32 * i_matrix->_13 * i_matrix->_44 +
   i_matrix->_31 * i_matrix->_12 * i_matrix->_23 * i_matrix->_44 - i_matrix->_11 * i_matrix->_32 * i_matrix->_23 * i_matrix->_44 - i_matrix->_21 * i_matrix->_12 * i_matrix->_33 * i_matrix->_44 + i_matrix->_11 * i_matrix->_22 * i_matrix->_33 * i_matrix->_44 ;
}

AN8XMATRIX * AN8XMatrixInverse(AN8XMATRIX * o_Matrix,float* o_Determinant,AN8XMATRIX * i_Matrix)
{
	float determinant = AN8XMatrixDeterminant(i_Matrix);
	float scale = 1.0f/ determinant;

	AN8XMATRIX tempMatrix;
    tempMatrix._11 = (i_Matrix->_32*i_Matrix->_43*i_Matrix->_24 - i_Matrix->_42*i_Matrix->_33*i_Matrix->_24 + i_Matrix->_42*i_Matrix->_23*i_Matrix->_34 - i_Matrix->_22*i_Matrix->_43*i_Matrix->_34 - i_Matrix->_32*i_Matrix->_23*i_Matrix->_44 + i_Matrix->_22*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._21 = (i_Matrix->_41*i_Matrix->_33*i_Matrix->_24 - i_Matrix->_31*i_Matrix->_43*i_Matrix->_24 - i_Matrix->_41*i_Matrix->_23*i_Matrix->_34 + i_Matrix->_21*i_Matrix->_43*i_Matrix->_34 + i_Matrix->_31*i_Matrix->_23*i_Matrix->_44 - i_Matrix->_21*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._31 = (i_Matrix->_31*i_Matrix->_42*i_Matrix->_24 - i_Matrix->_41*i_Matrix->_32*i_Matrix->_24 + i_Matrix->_41*i_Matrix->_22*i_Matrix->_34 - i_Matrix->_21*i_Matrix->_42*i_Matrix->_34 - i_Matrix->_31*i_Matrix->_22*i_Matrix->_44 + i_Matrix->_21*i_Matrix->_32*i_Matrix->_44)*scale;
    tempMatrix._41 = (i_Matrix->_41*i_Matrix->_32*i_Matrix->_23 - i_Matrix->_31*i_Matrix->_42*i_Matrix->_23 - i_Matrix->_41*i_Matrix->_22*i_Matrix->_33 + i_Matrix->_21*i_Matrix->_42*i_Matrix->_33 + i_Matrix->_31*i_Matrix->_22*i_Matrix->_43 - i_Matrix->_21*i_Matrix->_32*i_Matrix->_43)*scale;

	tempMatrix._12 = (i_Matrix->_42*i_Matrix->_33*i_Matrix->_14 - i_Matrix->_32*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_42*i_Matrix->_13*i_Matrix->_34 + i_Matrix->_12*i_Matrix->_43*i_Matrix->_34 + i_Matrix->_32*i_Matrix->_13*i_Matrix->_44 - i_Matrix->_12*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._22 = (i_Matrix->_31*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_33*i_Matrix->_14 + i_Matrix->_41*i_Matrix->_13*i_Matrix->_34 - i_Matrix->_11*i_Matrix->_43*i_Matrix->_34 - i_Matrix->_31*i_Matrix->_13*i_Matrix->_44 + i_Matrix->_11*i_Matrix->_33*i_Matrix->_44)*scale;
    tempMatrix._32 = (i_Matrix->_41*i_Matrix->_32*i_Matrix->_14 - i_Matrix->_31*i_Matrix->_42*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_12*i_Matrix->_34 + i_Matrix->_11*i_Matrix->_42*i_Matrix->_34 + i_Matrix->_31*i_Matrix->_12*i_Matrix->_44 - i_Matrix->_11*i_Matrix->_32*i_Matrix->_44)*scale;
    tempMatrix._42 = (i_Matrix->_31*i_Matrix->_42*i_Matrix->_13 - i_Matrix->_41*i_Matrix->_32*i_Matrix->_13 + i_Matrix->_41*i_Matrix->_12*i_Matrix->_33 - i_Matrix->_11*i_Matrix->_42*i_Matrix->_33 - i_Matrix->_31*i_Matrix->_12*i_Matrix->_43 + i_Matrix->_11*i_Matrix->_32*i_Matrix->_43)*scale;

	tempMatrix._13 = (i_Matrix->_22*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_42*i_Matrix->_23*i_Matrix->_14 + i_Matrix->_42*i_Matrix->_13*i_Matrix->_24 - i_Matrix->_12*i_Matrix->_43*i_Matrix->_24 - i_Matrix->_22*i_Matrix->_13*i_Matrix->_44 + i_Matrix->_12*i_Matrix->_23*i_Matrix->_44)*scale;
    tempMatrix._23 = (i_Matrix->_41*i_Matrix->_23*i_Matrix->_14 - i_Matrix->_21*i_Matrix->_43*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_13*i_Matrix->_24 + i_Matrix->_11*i_Matrix->_43*i_Matrix->_24 + i_Matrix->_21*i_Matrix->_13*i_Matrix->_44 - i_Matrix->_11*i_Matrix->_23*i_Matrix->_44)*scale;
    tempMatrix._33 = (i_Matrix->_21*i_Matrix->_42*i_Matrix->_14 - i_Matrix->_41*i_Matrix->_22*i_Matrix->_14 + i_Matrix->_41*i_Matrix->_12*i_Matrix->_24 - i_Matrix->_11*i_Matrix->_42*i_Matrix->_24 - i_Matrix->_21*i_Matrix->_12*i_Matrix->_44 + i_Matrix->_11*i_Matrix->_22*i_Matrix->_44)*scale;
    tempMatrix._43 = (i_Matrix->_41*i_Matrix->_22*i_Matrix->_13 - i_Matrix->_21*i_Matrix->_42*i_Matrix->_13 - i_Matrix->_41*i_Matrix->_12*i_Matrix->_23 + i_Matrix->_11*i_Matrix->_42*i_Matrix->_23 + i_Matrix->_21*i_Matrix->_12*i_Matrix->_43 - i_Matrix->_11*i_Matrix->_22*i_Matrix->_43)*scale;

	tempMatrix._14 = (i_Matrix->_32*i_Matrix->_23*i_Matrix->_14 - i_Matrix->_22*i_Matrix->_33*i_Matrix->_14 - i_Matrix->_32*i_Matrix->_13*i_Matrix->_24 + i_Matrix->_12*i_Matrix->_33*i_Matrix->_24 + i_Matrix->_22*i_Matrix->_13*i_Matrix->_34 - i_Matrix->_12*i_Matrix->_23*i_Matrix->_34)*scale;
    tempMatrix._24 = (i_Matrix->_21*i_Matrix->_33*i_Matrix->_14 - i_Matrix->_31*i_Matrix->_23*i_Matrix->_14 + i_Matrix->_31*i_Matrix->_13*i_Matrix->_24 - i_Matrix->_11*i_Matrix->_33*i_Matrix->_24 - i_Matrix->_21*i_Matrix->_13*i_Matrix->_34 + i_Matrix->_11*i_Matrix->_23*i_Matrix->_34)*scale;
    tempMatrix._34 = (i_Matrix->_31*i_Matrix->_22*i_Matrix->_14 - i_Matrix->_21*i_Matrix->_32*i_Matrix->_14 - i_Matrix->_31*i_Matrix->_12*i_Matrix->_24 + i_Matrix->_11*i_Matrix->_32*i_Matrix->_24 + i_Matrix->_21*i_Matrix->_12*i_Matrix->_34 - i_Matrix->_11*i_Matrix->_22*i_Matrix->_34)*scale;
    tempMatrix._44 = (i_Matrix->_21*i_Matrix->_32*i_Matrix->_13 - i_Matrix->_31*i_Matrix->_22*i_Matrix->_13 + i_Matrix->_31*i_Matrix->_12*i_Matrix->_23 - i_Matrix->_11*i_Matrix->_32*i_Matrix->_23 - i_Matrix->_21*i_Matrix->_12*i_Matrix->_33 + i_Matrix->_11*i_Matrix->_22*i_Matrix->_33)*scale;

	*o_Matrix = tempMatrix;

	if( o_Determinant != NULL )
		*o_Determinant = determinant;

	return o_Matrix;
}

AN8XBOUNDINGBOX* AN8XMoveBoundingBox(AN8XBOUNDINGBOX* i_box, AN8XVECTOR3* i_Translation, AN8XQUATERNION* i_Orientation)
{
	AN8XMATRIX matrix;
	AN8XMatrixRotationQuaternion(&matrix,i_Orientation);

	AN8XVECTOR3 boxVertices[8];

	boxVertices[0] = i_box->Min;
	boxVertices[1] = i_box->Min;boxVertices[1].y = i_box->Max.y;

	boxVertices[2] = i_box->Min;boxVertices[2].x = i_box->Max.x;
	boxVertices[3] = boxVertices[2];boxVertices[3].y = i_box->Max.y;

	boxVertices[4] = i_box->Max;boxVertices[4].x = i_box->Min.x;
	boxVertices[5] = i_box->Max;boxVertices[4].x = i_box->Min.x;
	boxVertices[5].y = i_box->Min.y;

	boxVertices[6] = i_box->Max;
	boxVertices[7] = i_box->Max;boxVertices[7].y = i_box->Min.y;

	for( int i = 0 ; i < 8 ; ++i)
	{
		AN8XVECTOR4 vec4;
		AN8XVec3Transform(&vec4,&boxVertices[i],&matrix);
		boxVertices[i].x = vec4.x;
		boxVertices[i].y = vec4.y;
		boxVertices[i].z = vec4.z;

		boxVertices[i] = boxVertices[i] + *i_Translation;
		boxVertices[i] = boxVertices[i] + *i_Translation;

		if( boxVertices[i].x < i_box->Min.x )
			i_box->Min.x = boxVertices[i].x;

		if( boxVertices[i].y < i_box->Min.y )
			i_box->Min.y = boxVertices[i].y;

		if( boxVertices[i].z < i_box->Min.z )
			i_box->Min.z = boxVertices[i].z;
	}

	return i_box;
}


void AN8XYawPitchRollRotationMatrix(double* o_Yaw, double* o_Pitch, double* o_Roll, AN8XMATRIX* i_Matrix)
{
   double threshold = 0.001; 

   *o_Pitch = asin(-i_Matrix->_32);
   double cosPitch      = cos(*o_Pitch);


   if(cosPitch > threshold) 
   {
      *o_Roll = atan2(i_Matrix->_12, i_Matrix->_22);
      *o_Yaw  = atan2(i_Matrix->_31, i_Matrix->_33);
   }
   else 
   {
      *o_Roll = atan2(-i_Matrix->_21, i_Matrix->_11);
      *o_Yaw  = 0.0;
   }
} 

void AN8XYawPitchRollQuaternion(double* o_Yaw, double* o_Pitch, double* o_Roll, AN8XQUATERNION* i_Quat )
{
	double testSingularity = i_Quat->x*i_Quat->y + i_Quat->z*i_Quat->w;
	if (testSingularity > 0.499) 
	{
		*o_Yaw = 2 * atan2(i_Quat->x,i_Quat->w);
		*o_Roll = AN8X_PI/2;
		*o_Pitch = 0;
		return;
	}
	if (testSingularity < -0.499) 
	{ 
		*o_Yaw = -2 * atan2(i_Quat->x,i_Quat->w);
		*o_Roll = - AN8X_PI/2;
		*o_Pitch = 0;
		return;
	}
    double sqx = i_Quat->x*i_Quat->x;
    double sqy = i_Quat->y*i_Quat->y;
    double sqz = i_Quat->z*i_Quat->z;

	double qx = i_Quat->x;
	double qy = i_Quat->y;
	double qz = i_Quat->z;
	double qw = i_Quat->w;

    *o_Yaw = atan2( 2.0 * qy * qw - 2.0 * qx * qz ,1.0 - 2.0*sqy - 2.0*sqz); // Y
	*o_Roll = asin(2*testSingularity); // Z
	*o_Pitch = atan2(2*qx*qw-2*qy*qz , 1 - 2*sqx - 2*sqz); // X
}

void AN8XBaseConcatenate(AN8XVECTOR3* o_Origin, AN8XQUATERNION* o_Orientation,
					AN8XVECTOR3* i_OriginParent, AN8XQUATERNION* i_OrientationParent,
					AN8XVECTOR3* i_OriginChild, AN8XQUATERNION* i_OrientationChild)
{
	// Compute Parent Matrices
	AN8XMATRIX matRotGroup;
	AN8XMatrixRotationQuaternion(&matRotGroup,i_OrientationParent);
	AN8XMATRIX matTranslatGroup;
	matTranslatGroup._41 = i_OriginParent->x;
	matTranslatGroup._42 = i_OriginParent->y;
	matTranslatGroup._43 = i_OriginParent->z;

	// Compute Child Matrices
	AN8XMATRIX matRotElem;
	AN8XMatrixRotationQuaternion(&matRotElem,i_OrientationChild);
	AN8XMATRIX matTranslatElem;
	matTranslatElem._41 = i_OriginChild->x;
	matTranslatElem._42 = i_OriginChild->y;
	matTranslatElem._43 = i_OriginChild->z;

	// Compute final Matrix
	AN8XMATRIX matrix;
	//matrix = matRotElem * matTranslatElem * matRotGroup * matTranslatGroup;
	AN8XMatrixMultiply(&matrix,&matTranslatGroup,&matRotGroup);
	AN8XMatrixMultiply(&matrix,&matrix,&matTranslatElem);
	AN8XMatrixMultiply(&matrix,&matrix,&matRotElem);
			
	// Compute Final Origin
	o_Origin->x = matrix._41;
	o_Origin->y = matrix._42;
	o_Origin->z = matrix._43;
			
	// Convert Rotation matrix to final quaternion (orientation)
	AN8XQuaternionMatrixRotation(o_Orientation,&matrix);
}