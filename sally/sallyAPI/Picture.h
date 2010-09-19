////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Picture.h
///
/// \brief	Declares the picture class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

#include <d3dx9.h>
#include <dshow.h>
#include <GdiPlus.h>
#include "Game.h"

#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CPicture
		///
		/// \brief	Picture. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CPicture
		{
		private:
			//Define a custom vertex for our panel
			struct PANEL_CUSTOMVERTEX
			{
				FLOAT x, y, z;		//Position of vertex
				DWORD colour;		//Colour of vertex
				FLOAT u, v;			//Texture coordinates
			};

			SallyAPI::Core::CTexture*	m_pTexture;
			LPDIRECT3DVERTEXBUFFER9		m_pVertexBuffer;

			int		m_iWidth;
			int		m_iHeight;
			int		m_iAlphaBlending;
			
			float   m_fAngleY;
			float   m_fAngleX;
			float   m_fAngleZ;

			void	CleanUp();
			void	UpdateVertices();
			void	CreateVertexBuffer();
			
			void	MoveTo(int x, int y);
			void	Rotate(int x, int y);
		public:
			CPicture();
			~CPicture();

			SallyAPI::Core::CTexture*	GetTexture();

			void	SetTexture(SallyAPI::Core::CTexture* texture);

			bool	CreateEmptyPixelFormat(int iWidth, int iHeight, Gdiplus::PixelFormat pixelFormat);
			bool	CreateEmptyD3DFormat(int iWidth, int iHeight, D3DFORMAT d3dFormat);
			bool	CreateEmptyAsRenderTargetD3DFormat(int iWidth, int iHeight, D3DFORMAT d3dFormat);

			bool	LoadTexture(const std::string& fileName, bool createMipMaps = false);
			bool	LoadTextureOverGDI(const std::string& fileName, int width = -1, int height = -1);
			
			void	Draw(int x, int y);
			void	Draw(int x, int y, int width, int height);

			int		GetWidth();
			int		GetHeight();

			void	SetWidth(int width);
			void	SetHeight(int height);

			void	UpdateDimensions();

			void	SetAlphaBlending(int alphaBlending);

			void	SetRotationAngleY(float angle);
			void	SetRotationAngleX(float angle);
			void	SetRotationAngleZ(float angle);
		};
	}
}