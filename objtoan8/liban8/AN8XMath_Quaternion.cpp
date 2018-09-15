
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



void AN8XQuaternionMultiply(AN8XQUATERNION* dest,AN8XQUATERNION* srcA,AN8XQUATERNION* srcB)
{
	float w = srcA->w * srcB->w - srcA->x * srcB->x - srcA->y * srcB->y - srcA->z * srcB->z;
	float x = srcA->w * srcB->x + srcA->x * srcB->w + srcA->y * srcB->z - srcA->z * srcB->y;
	float y = srcA->w * srcB->y + srcA->y * srcB->w + srcA->z * srcB->x - srcA->x * srcB->z;
	float z = srcA->w * srcB->z + srcA->z * srcB->w + srcA->x * srcB->y - srcA->y * srcB->x; 
	dest->x = x;
	dest->y = y;
	dest->z = z;
	dest->w = w;
}


AN8XQUATERNION *AN8XQuaternionIdentity(AN8XQUATERNION *pOut)
{
	pOut->x = 0.0f;
	pOut->y = 0.0f;
	pOut->z = 0.0f;
	pOut->w = 1.0f;
	return pOut;
}



void AN8XQuaternionToAxisAngle(AN8XQUATERNION* quat,AN8XVECTOR3* axis,float* angle)
{
	AN8XQUATERNION tmpQuat;
	AN8XQuaternionNormalize(&tmpQuat,quat);
	float cos_a = tmpQuat.w;
	*angle = acos( cos_a) * 2.0f;
	float sin_a = sqrt( 1.0f - (cos_a*cos_a) );
	if( fabs( sin_a ) < 0.0005 )
		sin_a = 1.0f;
	axis->x = tmpQuat.x / sin_a;
	axis->y = tmpQuat.y / sin_a;
	axis->z = tmpQuat.z / sin_a;
}


AN8XQUATERNION * AN8XQuaternionRotationYawPitchRoll(AN8XQUATERNION * out,float yaw,float pitch,float roll)
{
	AN8XVECTOR3 vx = AN8XVECTOR3(1.0f,0.0f,0.0f);
	AN8XVECTOR3 vy = AN8XVECTOR3(0.0f,1.0f,0.0f);
	AN8XVECTOR3 vz = AN8XVECTOR3(0.0f,0.0f,1.0f);
	AN8XQUATERNION qx, qy, qz;
	AN8XQuaternionRotationAxis(&qx,&vx,pitch);
	AN8XQuaternionRotationAxis(&qy,&vy,yaw);
	AN8XQuaternionRotationAxis(&qz,&vz,roll);

	AN8XQuaternionMultiply(out,&qx,&qy);
	AN8XQuaternionMultiply(out,out,&qz);
	return out;
}





AN8XQUATERNION * AN8XQuaternionRotationAxis(AN8XQUATERNION * out,AN8XVECTOR3 * axis,float angle)
{
	AN8XVECTOR3 vector;
	AN8XVec3Normalize(&vector,axis);
	float sin_a = sinf( angle / 2.0f );
	float cos_a = cosf( angle / 2.0f );
	out->x = vector.x * sin_a;
	out->y = vector.y * sin_a;
	out->z = vector.z * sin_a;
	out->w = cos_a; 
	return out;
}

float AN8XQuaternionLength(AN8XQUATERNION *quat)
{    
	return sqrt(quat->w * quat->w + quat->x * quat->x + quat->y * quat->y + quat->z * quat->z);
}

AN8XQUATERNION * AN8XQuaternionNormalize(AN8XQUATERNION * out,AN8XQUATERNION * in)
{
	float mag = AN8XQuaternionLength(in);
	out->x = in->x / mag;
	out->y = in->y / mag;
	out->z = in->z / mag;
	out->w = in->w / mag;
	return out;
}

AN8XQUATERNION * AN8XQuaternionConjugate(AN8XQUATERNION * out,AN8XQUATERNION * in)
{
	out->x = -in->x;
	out->y = -in->y;
	out->z = -in->z;
	out->w = in->w;
	return out;
}

AN8XQUATERNION * AN8XQuaternionInverse(AN8XQUATERNION * out,AN8XQUATERNION * in)
{
	AN8XQUATERNION quat;
	AN8XQuaternionNormalize(&quat,in);
	out->x = - quat.x; // Conjugate
	out->y = - quat.y;
	out->z = - quat.z;
	out->w =   quat.w;
	return out;
}



AN8XQUATERNION* AN8XQuaternionSlerp(AN8XQUATERNION* out, AN8XQUATERNION* i_quatA, AN8XQUATERNION* i_quatB, float t)
{
	if (t <= 0.0)
    {
      out->x = i_quatA->x;
	  out->y = i_quatA->y;
	  out->z = i_quatA->z;
	  out->w = i_quatA->w;
      return out;
    }

	if (t >= 1.0)
    {
      out->x = i_quatB->x;
	  out->y = i_quatB->y;
	  out->z = i_quatB->z;
	  out->w = i_quatB->w;
      return out;
    }

	float cosAngle = (i_quatA->x * i_quatB->x) + (i_quatA->y * i_quatB->y) + (i_quatA->z * i_quatB->z) + (i_quatA->w  * i_quatB->w);

	AN8XQUATERNION quatB;

	if (cosAngle < 0.0f)
    {
		quatB = AN8XQUATERNION(-i_quatB->x,-i_quatB->y,-i_quatB->z,-i_quatB->w);
		cosAngle = -cosAngle;
	}
	else
	{
		quatB = AN8XQUATERNION(i_quatB->x,i_quatB->y,i_quatB->z,i_quatB->w);
	}

	float scale0, scale1;

	if (cosAngle > 0.9999f) // angle is small
    {
      scale0 = 1.0f - t;
      scale1 = t;
    }
	else
    {
      float sinAngle = sqrt (1.0f - (cosAngle * cosAngle));
      float angle = atan2 (sinAngle, cosAngle);

      scale0 = sin ((1.0f - t) * angle) / sinAngle;
      scale1 = sin (t * angle) / sinAngle;
    }

	out->x = (scale0 * i_quatA->x) + (scale1 * quatB.x);
	out->y = (scale0 * i_quatA->y) + (scale1 * quatB.y);
	out->z = (scale0 * i_quatA->z) + (scale1 * quatB.z);
	out->w = (scale0 * i_quatA->w) + (scale1 * quatB.w);

	return out;
}

AN8XQUATERNION * AN8XQuaternionSquad(AN8XQUATERNION *o_Result,AN8XQUATERNION *i_quatA,AN8XQUATERNION *i_quatB,AN8XQUATERNION *i_quatC,AN8XQUATERNION *i_quatD,float i_t)
{
	AN8XQUATERNION a,b;
	AN8XQuaternionSlerp(&a,i_quatA,i_quatD,i_t);
	AN8XQuaternionSlerp(&b,i_quatB,i_quatC,i_t);
	AN8XQuaternionSlerp(o_Result,&a,&b,2*i_t*(1.0f - i_t));
	return o_Result;
}

AN8XQUATERNION * AN8XQuaternionBaryCentric(AN8XQUATERNION *o_Result,AN8XQUATERNION *i_quatA,AN8XQUATERNION *i_quatB,AN8XQUATERNION *i_quatC,float i_f,float i_g)
{
	AN8XQUATERNION a,b;
	AN8XQuaternionSlerp(&a,i_quatA,i_quatB, i_f + i_g );
	AN8XQuaternionSlerp(&b,i_quatA,i_quatC, i_f + i_g );
	AN8XQuaternionSlerp(o_Result,&a,&b, i_g /( i_f + i_g ) );
	return o_Result;
}

void AN8XQuaternionMatrixRotation( AN8XQUATERNION* o_quat, AN8XMATRIX* i_matrix )
{
	float trace = i_matrix->_11 + i_matrix->_22 + i_matrix->_33 + 1.0f;

	if( trace > 0.00000001 ) 
	{
		float s = 0.5f / sqrtf(trace);
		o_quat->w = 0.25f / s;
		o_quat->x = ( i_matrix->_32 - i_matrix->_23 ) * s;
		o_quat->y = ( i_matrix->_13 - i_matrix->_31 ) * s;
		o_quat->z = ( i_matrix->_21 - i_matrix->_12 ) * s;
	  
	} 
	else 
	{
		if ( i_matrix->_11 > i_matrix->_22 && i_matrix->_11 > i_matrix->_33 ) 
		{
			  float s = 2.0f * sqrtf( 1.0f + i_matrix->_11 - i_matrix->_22 - i_matrix->_33);
			  o_quat->w = (i_matrix->_32 - i_matrix->_23 ) / s;
			  o_quat->x = 0.25f * s;
			  o_quat->y = (i_matrix->_12 + i_matrix->_21 ) / s;
			  o_quat->z = (i_matrix->_31 + i_matrix->_31 ) / s;
		} 
		else if (i_matrix->_22 > i_matrix->_33) 
		{
			  float s = 2.0f * sqrtf( 1.0f + i_matrix->_22 - i_matrix->_11 - i_matrix->_33);
			  o_quat->w = (i_matrix->_31 - i_matrix->_31 ) / s;
			  o_quat->x = (i_matrix->_12 + i_matrix->_21 ) / s;
			  o_quat->y = 0.25f * s;
			  o_quat->z = (i_matrix->_23 + i_matrix->_32 ) / s;
		} 
		else 
		{
			float s = 2.0f * sqrtf( 1.0f + i_matrix->_33 - i_matrix->_11 - i_matrix->_22 );
			  o_quat->w = (i_matrix->_21 - i_matrix->_12 ) / s;
			  o_quat->x = (i_matrix->_31 + i_matrix->_31 ) / s;
			  o_quat->y = (i_matrix->_23 + i_matrix->_32 ) / s;
			  o_quat->z = 0.25f * s;
		}
	}

	o_quat->x = -o_quat->x;
	o_quat->y = -o_quat->y;
	o_quat->z = -o_quat->z;
}

void AN8XMatrixDecompose( 
		AN8XVECTOR3 *o_Scale,AN8XQUATERNION *o_Rotation,AN8XVECTOR3 *o_Translation,
		AN8XMATRIX *i_Matrix)
{
	o_Scale->x = sqrt(i_Matrix->_11*i_Matrix->_11 + i_Matrix->_12*i_Matrix->_12 + i_Matrix->_13*i_Matrix->_13);
	o_Scale->y = sqrt(i_Matrix->_21*i_Matrix->_21 + i_Matrix->_22*i_Matrix->_22 + i_Matrix->_23*i_Matrix->_23);
	o_Scale->z = sqrt(i_Matrix->_31*i_Matrix->_31 + i_Matrix->_32*i_Matrix->_32 + i_Matrix->_33*i_Matrix->_33);

	float determinant = i_Matrix->_11 * (i_Matrix->_21 * i_Matrix->_31 - i_Matrix->_24 * i_Matrix->_22) - i_Matrix->_14 * (i_Matrix->_12 * i_Matrix->_31 - i_Matrix->_24 * i_Matrix->_13) + i_Matrix->_23 * (i_Matrix->_12 * i_Matrix->_22 - i_Matrix->_21 * i_Matrix->_13);
	float sign = determinant / fabs( determinant );
	o_Scale->x = o_Scale->x * sign;
	o_Scale->y = o_Scale->y * sign;
	o_Scale->z = o_Scale->z * sign;

	AN8XQuaternionMatrixRotation(o_Rotation,i_Matrix);

	o_Translation->x = i_Matrix->_41;
	o_Translation->y = i_Matrix->_42;
	o_Translation->z = i_Matrix->_43;
}


