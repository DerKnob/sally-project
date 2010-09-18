////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ThemePluginImage.cpp
///
/// \brief	Implements the theme plugin image class. 
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

#include "ThemePluginImage.h"

using namespace SallyAPI::Config;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CThemePluginImage::CThemePluginImage(const std::string& pluginPath,
/// const std::string& pluginName, const std::string& filename, bool createMipMaps)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	pluginPath		Full pathname of the plugin file. 
/// \param	pluginName		Name of the plugin. 
/// \param	filename		Filename of the file. 
/// \param	createMipMaps	true to create mip maps. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CThemePluginImage::CThemePluginImage(const std::string& pluginPath, const std::string& pluginName, 
									 const std::string& filename, bool createMipMaps)
:m_strPluginPath(pluginPath), m_strPluginName(pluginName), m_strFilename(filename), m_bCreateMipMaps(createMipMaps)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CThemePluginImage::~CThemePluginImage()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CThemePluginImage::~CThemePluginImage()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CThemePluginImage::GetPluginPath()
///
/// \brief	Gets the plugin path. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The plugin path. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	CThemePluginImage::GetPluginPath()
{
	return m_strPluginPath;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CThemePluginImage::GetPluginName()
///
/// \brief	Gets the plugin name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The plugin name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	CThemePluginImage::GetPluginName()
{
	return m_strPluginName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CThemePluginImage::GetFilename()
///
/// \brief	Gets the filename. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The filename. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	CThemePluginImage::GetFilename()
{
	return m_strFilename;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CThemePluginImage::GetMipMaps()
///
/// \brief	Gets the mip maps. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CThemePluginImage::GetMipMaps()
{
	return m_bCreateMipMaps;
}