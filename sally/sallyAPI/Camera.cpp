////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Camera.cpp
///
/// \brief	Implements the camera class. 
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
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "camera.h"

using namespace SallyAPI::Core;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCamera::CCamera(int screenWidth, int screenHeight)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	screenWidth		Width of the screen. 
/// \param	screenHeight	Height of the screen. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CCamera::CCamera(int screenWidth, int screenHeight)
	:m_iScreenWidth(screenWidth), m_iScreenHeight(screenHeight), m_eCameraSetup(CAMERA_SETUP_UNKONW)
{
	//LPDIRECT3DDEVICE9 pDirect3DDevice = SallyAPI::Core::CGame::GetDevice();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCamera::~CCamera()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCamera::~CCamera()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCamera::SetupGUI3DCamera()
///
/// \brief	Sets up the graphical user interface 3 d camera. 
///
/// \author	Christian Knobloch
/// \date	02.04.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCamera::SetupGUI3DCamera()
{
	if (m_eCameraSetup == CAMERA_SETUP_GUI3D)
		return;

	LPDIRECT3DDEVICE9 pDirect3DDevice = SallyAPI::Core::CGame::GetDevice();

	// The aspect ratio of how many horizontal pixels are used per vertical pixel
	float aspect = (float)m_iScreenWidth / (float)m_iScreenHeight; 

	float calculatedZValue = 0.0;
	float a = (float)m_iScreenHeight/2;
	float c = (float) (a / sin(D3DXToRadian(22.5)));
	calculatedZValue = (float) (cos(D3DXToRadian(22.5)) * c);


	D3DXMatrixLookAtLH(&m_matView,
		&D3DXVECTOR3( 0.0f, 0.0f,-calculatedZValue ),	// the camera position
		&D3DXVECTOR3( 0.0f, 0.0f, 0.0f ),				// the look-at position
		&D3DXVECTOR3( 0.0f, 1.0f, 0.0f ));				// the up direction

	D3DXMatrixPerspectiveFovLH(&m_matProjection,
		D3DXToRadian(45),	// the horizontal field of view
		aspect,				// aspect ratio
		-calculatedZValue,	// the near view-plane
		1.0f);				// the far view-plane

	pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &m_matProjection);    // set the projection
	pDirect3DDevice->SetTransform(D3DTS_VIEW, &m_matView);    // set the view transform to matView

	//Set the D3DRS_NORMALIZENORMALS render state to fix the problem when scaling the objects get darker
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	// Set up the render states 
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_eCameraSetup = CAMERA_SETUP_GUI3D;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCamera::SetupGUI2DCamera()
///
/// \brief	Sets up the graphical user interface 2 d camera. 
///
/// \author	Christian Knobloch
/// \date	02.04.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCamera::SetupGUI2DCamera()
{
	if (m_eCameraSetup == CAMERA_SETUP_GUI2D)
		return;

	LPDIRECT3DDEVICE9 pDirect3DDevice = SallyAPI::Core::CGame::GetDevice();

	pDirect3DDevice->SetTransform(D3DTS_PROJECTION, &m_matViewSpriteInterface);
	pDirect3DDevice->SetTransform(D3DTS_VIEW, &m_matProjectionSpriteInterface);

	//Set the D3DRS_NORMALIZENORMALS render state to fix the problem when scaling the objects get darker
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDirect3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	// Set up the render states 
	pDirect3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pDirect3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	m_eCameraSetup = CAMERA_SETUP_GUI2D;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCamera::StartRender()
///
/// \brief	Starts a render. 
///
/// \author	Christian Knobloch
/// \date	03.04.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCamera::StartRender()
{
	LPDIRECT3DDEVICE9 pDirect3DDevice = SallyAPI::Core::CGame::GetDevice();

	pDirect3DDevice->GetTransform(D3DTS_PROJECTION, &m_matViewSpriteInterface);
	pDirect3DDevice->GetTransform(D3DTS_VIEW, &m_matProjectionSpriteInterface);
}