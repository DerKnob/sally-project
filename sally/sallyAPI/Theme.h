////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Theme.h
///
/// \brief	Declares the theme class. 
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
	namespace GUI
	{
		class CPicture;
	}
}

#pragma once
#include "Define.h"

#include <map>
#include "Font.h"
#include "Picture.h"
#include "ThemePluginImage.h"
#include "AutoLock.h"

namespace SallyAPI
{
	namespace Config
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CTheme
		///
		/// \brief	Theme. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CTheme
		{
		private:
			std::map<int, SallyAPI::GUI::CPicture*>				m_mGUICache;
			std::map<std::string, std::string>					m_mPropertyCache;
			std::map<int, std::string>							m_mAdvancedImages;
			std::map<int, SallyAPI::Config::CThemePluginImage*>	m_mPluginImages;

			SallyAPI::System::CCritSection					m_Lock;
			std::string										m_strThemePath;
			std::string										m_strThemeName;

			void		AddPicture(const std::string& filename, int id, bool createMipMaps = false);
		public:
			CTheme(const std::string& name);
			~CTheme();

			SallyAPI::GUI::CPicture*	GetPicture(int id);
			std::string					GetThemePath();
			std::string					GetThemeName();

			void AddPicture(const std::string& filename, int id, int graphicId, bool createMipMaps = false);
			void AddPicturePlugin(const std::string& pluginPath, const std::string& pluginName, 
							const std::string& filename, bool createMipMaps, int id);
			void RemovePicture(int id);

			int			GetPropertyInt(const std::string& component, const std::string& id,
								const std::string& defaultString = "Unknown");
			std::string	GetPropertyString(const std::string& component, const std::string& id,
								const std::string& defaultString = "Unknown");

			std::map<int, std::string>&								GetAdvancedImages();
			std::map<int, SallyAPI::Config::CThemePluginImage*>&	GetPluginImages();
		};
	}
}