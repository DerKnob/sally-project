////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\WorldTransform.cpp
///
/// \brief	Implements the world transform class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "WorldTransform.h"

#define RADN ((FLOAT)(3.141592654f /180)) 

CWorldTransform::CWorldTransform()
{
	Reset();
}

CWorldTransform::~CWorldTransform(){
}

D3DXMATRIX CWorldTransform::GetMatrix(){
	return ObjectMatrix;
}
void CWorldTransform::SetMatrix(D3DXMATRIX newMatrix){
	ObjectMatrix = newMatrix;
}
void CWorldTransform::Scale(float x, float y,float z){
	m_fWidthCorrection	= x;
	m_fHeightCorrection	= y;
	m_fDepthCorrection	= z;
	CalculateMatrix();
}

float CWorldTransform::GetXPosition (){
	return m_vPositionCalc.x;
}
float CWorldTransform::GetYPosition (){
	return m_vPositionCalc.y;
}
float CWorldTransform::GetZPosition (){
	return m_vPositionCalc.z;
}
void CWorldTransform::SetXPosition (float NewX){
	m_vPositionCalc.x = NewX;
   CalculateMatrix();
}
void CWorldTransform::SetYPosition (float NewY){
	m_vPositionCalc.y = NewY;
   CalculateMatrix();
}
void CWorldTransform::SetZPosition (float NewZ){
	m_vPositionCalc.z = NewZ;
   CalculateMatrix();
}
void CWorldTransform::SetPosition(D3DXVECTOR3 pos){
   m_vPositionCalc.x = pos.x;
   m_vPositionCalc.y = pos.y;
   m_vPositionCalc.z = pos.z;
   CalculateMatrix();
}
void CWorldTransform::MoveX (float ToMove){
	if(ToMove > 0){
	ObjectMatrix._41 += ToMove;
	}
	if(ToMove < 0){
	ObjectMatrix._41 += (-1 *ToMove);
	}
}
void CWorldTransform::MoveY (float ToMove){
	if(ToMove > 0){
	ObjectMatrix._42 += ToMove;
	}
	if(ToMove < 0){
	ObjectMatrix._42 += (-1 *ToMove);
	}
}
void CWorldTransform::MoveZ (float ToMove){
	if(ToMove > 0){
	ObjectMatrix._43 += ToMove;
	}
	if(ToMove < 0){
	ObjectMatrix._43 += (-1 *ToMove);
	}
}
void CWorldTransform::AngleX (float NewAngle){
	mAngleX = NewAngle;
	CalculateMatrix();

}
void CWorldTransform::AngleY (float NewAngle){
	mAngleY = NewAngle;
	CalculateMatrix();
}
void CWorldTransform::AngleZ (float NewAngle){
	mAngleZ = NewAngle;
	CalculateMatrix();
}

void CWorldTransform::AngleZY (float NewZAngle,float NewYAngle){
	mAngleZ = NewZAngle;
	mAngleY = NewYAngle;
	CalculateMatrix();
}
void CWorldTransform::RotateX (float ToRot){
	float x,y,z;
	x = ObjectMatrix._41;
	y = ObjectMatrix._42;
	z = ObjectMatrix._43;
	
	D3DXMATRIX tmpMatrix;

	D3DXMatrixIdentity(&tmpMatrix);
	
	D3DXMatrixRotationX(&tmpMatrix,(ToRot*(RADN)));

	D3DXMatrixMultiply(&ObjectMatrix,&ObjectMatrix,&tmpMatrix);
	
	ObjectMatrix._41 = x;
	ObjectMatrix._42 = y;
	ObjectMatrix._43 = z;

}
void CWorldTransform::RotateY (float ToRot){
	float x,y,z;
	x = ObjectMatrix._41;
	y = ObjectMatrix._42;
	z = ObjectMatrix._43;

	D3DXMATRIX tmpMatrix;

	D3DXMatrixIdentity(&tmpMatrix);
	
	D3DXMatrixRotationY(&tmpMatrix,(ToRot*(RADN)));
		
	D3DXMatrixMultiply(&ObjectMatrix,&ObjectMatrix,&tmpMatrix);

	ObjectMatrix._41 = x;
	ObjectMatrix._42 = y;
	ObjectMatrix._43 = z;

}
void CWorldTransform::RotateZ (float ToRot){
	float x,y,z;
	x = ObjectMatrix._41;
	y = ObjectMatrix._42;
	z = ObjectMatrix._43;

	D3DXMATRIX tmpMatrix;

	D3DXMatrixIdentity(&tmpMatrix);
	
	D3DXMatrixRotationZ(&tmpMatrix,(ToRot*(RADN)));
	
	D3DXMatrixMultiply(&ObjectMatrix,&ObjectMatrix,&tmpMatrix);


	ObjectMatrix._41 = x;
	ObjectMatrix._42 = y;
	ObjectMatrix._43 = z;

}
void CWorldTransform::CalculateMatrix()
{
	float CosRx, CosRy, CosRz;
	float SinRx, SinRy, SinRz;
	CosRx = (float)cos( mAngleX);
	CosRy = (float)cos( mAngleY);
	CosRz = (float)cos( mAngleZ);

	SinRx = (float)sin( mAngleX);
	SinRy = (float)sin( mAngleY);
	SinRz = (float)sin( mAngleZ);
	

	ObjectMatrix._11 = (m_fWidthCorrection * CosRy * CosRz);
	ObjectMatrix._12 = (m_fWidthCorrection * CosRy * SinRz);
	ObjectMatrix._13 = -(m_fWidthCorrection * SinRy);

	ObjectMatrix._21 = -(m_fHeightCorrection * CosRx * SinRz) + (m_fHeightCorrection * SinRx * SinRy * CosRz);
	ObjectMatrix._22 = (m_fHeightCorrection * CosRx * CosRz) + (m_fHeightCorrection * SinRx * SinRy * SinRz) ;
	ObjectMatrix._23 = (m_fHeightCorrection * SinRx * CosRy);

	ObjectMatrix._31 = (m_fDepthCorrection * SinRx * SinRz) + (m_fDepthCorrection * CosRx * SinRy * CosRz) ;
    ObjectMatrix._32 = -(m_fDepthCorrection * SinRx * CosRz) + (m_fDepthCorrection * CosRx * SinRy * SinRz);
	ObjectMatrix._33 = (m_fDepthCorrection * CosRx * CosRy);

	ObjectMatrix._41 = m_vPositionCalc.x;
	ObjectMatrix._42 = m_vPositionCalc.y;
	ObjectMatrix._43 = m_vPositionCalc.z;
	ObjectMatrix._44 = 1.0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Summary: Reset the matrices to default position.
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void CWorldTransform::Reset()
{
	
	// Matrix Object of the class
	D3DXMatrixIdentity(&ObjectMatrix);
	UseMatrix = false;
	// position of the object
	m_vPositionCalc = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	// Helper Objects
	mAngleX	= 0.0f;
	mAngleY	= 0.0f;
	mAngleZ	= 0.0f;
}