////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Player.h
///
/// \brief	Declares the player class. 
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
#include "WorldTransform.h"

const FLOAT PI = 3.141592654f;

class CPlayer : public CWorldTransform
{
public:
	CPlayer();
	~CPlayer();

	// Funktion
	void	CalculatePosition(float delta);
	void	Reset();

	void	AccelerateX(int accelerate);
	void	AccelerateY(int accelerate);
	void	AccelerateZ(int accelerate);
	void	TurnX(int accelerate);
	void	TurnY(int accelerate);
	void	TurnZ(int accelerate);

	int		accelerateX;
	int		accelerateY;
	int		accelerateZ;
	int		turnX;
	int		turnY;
	int		turnZ;

	// Flugverhalten
	float	m_iSpeed;
	float	m_iSpeedTurnX;
	float	m_iSpeedTurnY;
	float	m_iSpeedTurnZ;

	D3DXVECTOR3				m_vPosition;
	D3DXVECTOR3				m_vLookAt;
	D3DXVECTOR3				m_vUp;
};
