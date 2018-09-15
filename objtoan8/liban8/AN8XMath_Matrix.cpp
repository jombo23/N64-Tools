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



AN8XMATRIX * AN8XMatrixMultiplyTranspose(AN8XMATRIX *o_Matrix,AN8XMATRIX *i_Matrix1,AN8XMATRIX *i_Matrix2)
{
	AN8XMatrixMultiply(o_Matrix,i_Matrix1,i_Matrix2);
	AN8XMatrixTranspose(o_Matrix,o_Matrix);
	return o_Matrix;
}

AN8XMATRIX* AN8XMatrixRotationAxis( AN8XMATRIX* o_Matrix,AN8XVECTOR3* i_Vector, float i_Angle)
{
	AN8XQUATERNION quat;
	AN8XQuaternionRotationAxis(&quat,i_Vector,i_Angle);

	AN8XMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}

AN8XMATRIX* AN8XMatrixRotationX( AN8XMATRIX* o_Matrix, float i_Angle)
{
	AN8XQUATERNION quat;
	AN8XVECTOR3 vector(1.0f,0.0f,0.0f);
	AN8XQuaternionRotationAxis(&quat,&vector,i_Angle);

	AN8XMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}

AN8XMATRIX* AN8XMatrixRotationY( AN8XMATRIX* o_Matrix, float i_Angle)
{
	AN8XQUATERNION quat;
	AN8XVECTOR3 vector(0.0f,1.0f,0.0f);
	AN8XQuaternionRotationAxis(&quat,&vector,i_Angle);

	AN8XMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}

AN8XMATRIX* AN8XMatrixRotationZ( AN8XMATRIX* o_Matrix, float i_Angle)
{
	AN8XQUATERNION quat;
	AN8XVECTOR3 vector(0.0f,0.0f,1.0f);
	AN8XQuaternionRotationAxis(&quat,&vector,i_Angle);

	AN8XMatrixRotationQuaternion( o_Matrix, &quat);
	return o_Matrix;
}
