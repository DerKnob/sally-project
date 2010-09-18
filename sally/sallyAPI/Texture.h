////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Texture.h
///
/// \brief	Declares the texture class. 
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
#include "Game.h"

namespace SallyAPI
{
	namespace Core
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CTexture
		///
		/// \brief	Texture. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CTexture
		{
		private:
			std::string				m_strFileName;
			int						m_iId;
			int						m_iWidth;
			int						m_iHeight;
			LPDIRECT3DTEXTURE9		m_Texture;

			void	CleanUp();
		public:
			CTexture();
			virtual ~CTexture();

			bool	LoadTexture(const std::string& fileName, int id, bool createMipMaps = false);
			bool	CreateEmpty(int iWidth, int iHeight, int id, D3DFORMAT d3dFormat);
			bool	CreateEmptyRenderTarget(int iWidth, int iHeight, int id, D3DFORMAT d3dFormat);

			std::string			GetFileName();
			LPDIRECT3DTEXTURE9	GetTexture();
			int					GetWidth();
			int					GetHeight();
			int					GetId();
			void				SetTexture(LPDIRECT3DTEXTURE9		m_Texture);
		};
	}
}
