////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\WorldTransform.h
///
/// \brief	Declares the world transform class. 
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

#pragma once
#include "Define.h"
#include <d3dx9.h>

class CWorldTransform
	: public SallyAPI::Core::CBaseObject
{
public:
	CWorldTransform();
	virtual ~CWorldTransform();

	void	Scale(float x, float y,float z);
	float	GetXPosition ();
	float	GetYPosition ();
	float	GetZPosition ();
	void	SetXPosition (float NewX);
	void	SetYPosition (float NewY);
	void	SetZPosition (float NewZ);
	void	SetPosition(D3DXVECTOR3 pos);
	void	MoveX (float);
	void	MoveY (float);
	void	MoveZ (float);
	void	AngleX (float);
	void	AngleY (float);
	void	AngleZ (float);
	void	AngleZY (float,float);
	void	RotateX (float);
	void	RotateY (float);
	void	RotateZ (float);

	D3DXMATRIX GetMatrix();
	void SetMatrix(D3DXMATRIX);

	void	Reset();

	D3DXVECTOR3	GetPosition() { return m_vPositionCalc; }

protected:
	// Matrix Objekt der Klasse
	D3DXMATRIX ObjectMatrix;
	bool UseMatrix;
	// Position des Objektes
	D3DXVECTOR3 m_vPositionCalc;
	// Hilfsvariablen für die Scalierung
	float mScaleX;
	float mScaleY;
	float mScaleZ;
	// Hilfsvariablen für den Winkel des Objektes
	float mAngleX;
	float mAngleY;
	float mAngleZ;

	void CalculateMatrix();
};
