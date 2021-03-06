////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Camera.h
///
/// \brief	Declares the camera class. 
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

#pragma once
#include "Define.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <d3dx9.h>
#include "Game.h"

namespace SallyAPI
{
	namespace Core
	{
		enum CAMERA_SETUP {CAMERA_SETUP_UNKONW, CAMERA_SETUP_GUI2D, CAMERA_SETUP_GUI3D};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CCamera
		///
		/// \brief	Camera. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CCamera
		{
		private:
			D3DXMATRIX		m_matView;				// the view transform matrix
			D3DXMATRIX		m_matProjection;		// the projection transform matrix
			D3DXMATRIX		m_matViewSpriteInterface;
			D3DXMATRIX		m_matProjectionSpriteInterface;
			int				m_iScreenWidth;
			int				m_iScreenHeight;
			CAMERA_SETUP	m_eCameraSetup;
			RECT			m_pScissorRect;
		public:
			CCamera(int screenWidth, int screenHeight);
			~CCamera();

			void		SetupGUI2DCamera();
			void		SetupGUI3DCamera();

			void		StartRender();

			RECT		SetupScissorRect(RECT& rect);
			void		DisableScissorRect();
			RECT		GetScissorRect();
		};
	}
}