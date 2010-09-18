////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\TextureManager.h
///
/// \brief	Declares the texture manager class. 
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

namespace SallyAPI
{
	namespace Core
	{
		class CTexture;
	}
}

#pragma once
#include "Define.h"
#include <map>
#include "Texture.h"
#include "AutoLock.h"

namespace SallyAPI
{
	namespace Core
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CTextureManager
		///
		/// \brief	Manager for textures. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CTextureManager
		{
		private:
			static SallyAPI::Core::CTextureManager*		m_pObject;

			int	m_iCounter;
			std::vector<SallyAPI::Core::CTexture*>		m_vTextures;
			std::map<int, SallyAPI::Core::CTexture*>	m_mTexturesRenderTarget;
			SallyAPI::System::CCritSection				m_Lock;

			int			GetNextId();

			CTextureManager();
			~CTextureManager();
		public:
			static SallyAPI::Core::CTextureManager*		GetInstance();
			static void	DeleteInstance();

			SallyAPI::Core::CTexture* LoadTexture(const std::string& fileName, bool createMipMaps = false);
			SallyAPI::Core::CTexture* CreateEmptyTexture(int iWidth, int iHeight, D3DFORMAT d3dFormat);
			SallyAPI::Core::CTexture* CreateEmptyTextureAsRenderTarget(int iWidth, int iHeight, D3DFORMAT d3dFormat);
			
			SallyAPI::Core::CTexture* GetTextureById(int id);
			SallyAPI::Core::CTexture* GetTextureByName(const std::string& fileName);

			bool RemoveTextureById(int id);
			bool RemoveTextureByName(const std::string& fileName);
			bool RemoveTextureByTexture(SallyAPI::Core::CTexture* texture);
		};
	}
}