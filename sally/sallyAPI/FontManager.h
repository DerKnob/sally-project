////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FontManager.h
///
/// \brief	Declares the font manager class. 
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

namespace SallyAPI
{
	namespace Core
	{
		class CFont;
	}
}

#pragma once
#include "Define.h"
#include "Font.h"
#include <map>
#include <string>

namespace SallyAPI
{
	namespace Core
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CFontManager
		///
		/// \brief	Manager for fonts. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CFontManager
		{
		private:
			static SallyAPI::Core::CFontManager*			m_pObject;
			std::map<std::string, SallyAPI::Core::CFont*>	m_Fonts;

			void LoadFont(const std::string& fontName);

			CFontManager();
			~CFontManager();
		public:
			static SallyAPI::Core::CFontManager* GetInstance();
			static void	DeleteInstance();

			SallyAPI::Core::CFont* GetFontByName(const std::string& fontName);

			bool RemoveFontByName(const std::string& fontName);
			bool RemoveFontByFont(SallyAPI::Core::CFont* font);

			bool OnLostDevice();
			bool OnResetDevice();

			void Reload();
		};
	}
}