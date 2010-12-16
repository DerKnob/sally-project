////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ReloadThemeImage.cpp
///
/// \brief	Implements the reload theme image class. 
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

#include "ReloadThemeImage.h"

using namespace SallyAPI::Config;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CReloadThemeImage::CReloadThemeImage(SallyAPI::Config::CTheme* theme,
/// SallyAPI::Config::CThemePluginImage* themePluginImage, int id)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	theme				If non-null, the theme. 
/// \param [in,out]	themePluginImage	If non-null, the theme plugin image. 
/// \param	id							The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CReloadThemeImage::CReloadThemeImage(SallyAPI::Config::CTheme* theme,
									 SallyAPI::Config::CThemePluginImage* themePluginImage, int id)
{
	m_pTheme = theme;
	m_pThemePluginImage = themePluginImage;
	m_iId = id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CReloadThemeImage::CReloadThemeImage(SallyAPI::Config::CTheme* theme,
/// const std::string& filename, int id)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	theme	If non-null, the theme. 
/// \param	filename		Filename of the file. 
/// \param	id				The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CReloadThemeImage::CReloadThemeImage(SallyAPI::Config::CTheme* theme,
									 const std::string& filename, int id)
{
	m_pTheme = theme;
	m_strFilename = filename;
	m_iId = id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CReloadThemeImage::~CReloadThemeImage()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CReloadThemeImage::~CReloadThemeImage()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CReloadThemeImage::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CReloadThemeImage::RunEx()
{
	if (m_strFilename.length() == 0)
	{
		m_pTheme->AddPicturePlugin(m_pThemePluginImage->GetPluginPath(), m_pThemePluginImage->GetPluginName(),
			m_pThemePluginImage->GetFilename(), m_pThemePluginImage->GetMipMaps(), m_iId);
	}
	else
	{
		m_pTheme->AddPicture(m_strFilename, m_iId, 0);
	}
}