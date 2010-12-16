////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Player.cpp
///
/// \brief	Implements the player class. 
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

#include "Player.h"

CPlayer::CPlayer()
{
	Reset();
}

CPlayer::~CPlayer()
{
}

void CPlayer::Reset()
{
	CWorldTransform::Reset();

	m_iSpeed = 0;
	m_iSpeedTurnX = 0;
	m_iSpeedTurnY = 0;
	m_iSpeedTurnZ = 0;

	accelerateX = 0;
	accelerateY = 0;
	accelerateZ = 0;
	turnX = 0;
	turnY = 0;
	turnZ = 0;

	m_vPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_vLookAt = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
	m_vUp = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
}

void CPlayer::AccelerateX(int accelerate)
{
	accelerateX = accelerate;
}

void CPlayer::AccelerateY(int accelerate)
{
	accelerateY = accelerate;
}

void CPlayer::AccelerateZ(int accelerate)
{
	accelerateZ = accelerate;
}

void CPlayer::TurnX(int accelerate)
{
	turnX = accelerate;
}

void CPlayer::TurnY(int accelerate)
{
	turnY = accelerate;
}

void CPlayer::TurnZ(int accelerate)
{
	turnZ = accelerate;
}

void CPlayer::CalculatePosition(float delta)
{
	/* Calculate */
	// iSpeed
	//m_iSpeed += (0.2 * accelerateZ * delta * m_Ship->m_iAccelerate);
	m_iSpeed = delta * (5000 * accelerateZ);
	
	// TurnX
	if (turnX == -1)
		m_iSpeedTurnX = 0.5 * delta * 50;
	if (turnX == 1)
		m_iSpeedTurnX = -0.5 * delta * 50;
	
	// TurnY
	if (turnY == 1)
		m_iSpeedTurnY = 0.5 * delta * 50;
	if (turnY == -1)
		m_iSpeedTurnY = -0.5 * delta * 50;

	// TurnZ
	if (turnZ == 1)
		m_iSpeedTurnZ = 0.5 * delta * 50;
	if (turnZ == -1)
		m_iSpeedTurnZ = -0.5 * delta * 50;

	// Calculate Position
	// Richtung nach vorne bestimmen
	D3DXVECTOR3 vDirection;
	D3DXVec3Normalize(&vDirection,&(this->m_vLookAt - this->m_vPosition));

	this->m_vPosition	+= vDirection * m_iSpeed * delta;
	this->m_vLookAt		+= vDirection * m_iSpeed * delta;

	// Richtung nach vorne bestimmen
	//D3DXVECTOR3 vDirection;
	D3DXVec3Normalize(&vDirection,&(this->m_vLookAt - this->m_vPosition));

	// Richtung nach links bestimmen
	D3DXVECTOR3 vLeft;
	D3DXVec3Cross(&vLeft,&vDirection,&this->m_vUp);
	D3DXVec3Normalize(&vLeft,&vLeft);

	this->m_vPosition	+= vLeft * accelerateX * delta;
	this->m_vLookAt		+= vLeft * accelerateX * delta;

	SetXPosition(m_vPosition.x);
	SetYPosition(m_vPosition.y);
	SetZPosition(m_vPosition.z);
	AngleY(m_iSpeedTurnX * delta + mAngleY);
	AngleX(m_iSpeedTurnY * delta + mAngleX);
	RotateZ(m_iSpeedTurnZ * delta);

	// Calculate ViewMatrix
	//this->m_	-= delta * m_iSpeedTurnY; 
	//this->  -= delta * m_iSpeedTurnX;

	// Da wir den Kopf ja nicht ausrenken wollen, begrenzen
	// wir nach unten und nach oben den Winkel auf etwa 60°
	/*
	if (m_fPitch > m_Ship->m_iMaxPitch)
		m_fPitch = m_Ship->m_iMaxPitch;

	if (m_fPitch < m_Ship->m_iMinPitch)
		m_fPitch = m_Ship->m_iMinPitch;
	*/
	// Richtungsvektor ermitteln
	//D3DXVECTOR3 vDirection;

	vDirection.x = cos(this->mAngleY)*cos(this->mAngleX);
	vDirection.y = sin(this->mAngleX);
	vDirection.z = sin(this->mAngleY)*cos(this->mAngleX);

	// Blickrichtung neu berechnen
	this->m_vLookAt = this->m_vPosition + vDirection;

	// Reset
	accelerateX = 0;
	accelerateY = 0;
	accelerateZ = 0;
	turnX = 0;
	turnY = 0;
	turnZ = 0;
	m_iSpeed = 0;
	m_iSpeedTurnX = 0;
	m_iSpeedTurnY = 0;
	m_iSpeedTurnZ = 0;
}