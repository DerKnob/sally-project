////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Theme.cpp
///
/// \brief	Implements the theme class. 
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

#include "Theme.h"

using namespace SallyAPI::Config;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTheme::CTheme(const std::string& name)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	name	The name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CTheme::CTheme(const std::string& name)
{
	m_strThemePath = SallyAPI::System::SystemHelper::GetModulePath();

	m_strThemePath.append("Themes\\");
	m_strThemePath.append(name);
	m_strThemePath.append("\\");

	m_strThemeName = GetPropertyString("info", "name");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTheme::~CTheme()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTheme::~CTheme()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::map<int, SallyAPI::GUI::CPicture*>::iterator iterGUI;
	SallyAPI::GUI::CPicture* gui;

	for(iterGUI = m_mGUICache.begin(); iterGUI != m_mGUICache.end(); ++iterGUI)
	{
		gui = iterGUI->second;
		if (gui != NULL)
			delete (gui);
	}

	std::map<int, SallyAPI::Config::CThemePluginImage*>::iterator iterPlugins = m_mPluginImages.begin();
	SallyAPI::Config::CThemePluginImage* plugin;

	while (iterPlugins != m_mPluginImages.end())
	{
		plugin = iterPlugins->second;
		if (plugin != NULL)
			delete (plugin);
		++iterPlugins;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::map<int, std::string>& CTheme::GetAdvancedImages()
///
/// \brief	Gets the advanced images. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The advanced images. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<int, std::string>& CTheme::GetAdvancedImages()
{
	return m_mAdvancedImages;
}

std::map<int, SallyAPI::Config::CThemePluginImage*>& CTheme::GetPluginImages()
{
	return m_mPluginImages;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CTheme::GetThemePath()
///
/// \brief	Gets the theme path. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The theme path. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	CTheme::GetThemePath()
{
	return m_strThemePath;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CTheme::GetThemeName()
///
/// \brief	Gets the theme name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The theme name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string	CTheme::GetThemeName()
{
	return m_strThemeName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTheme::GetPropertyInt(const std::string& component, const std::string& id,
/// const std::string& defaultString)
///
/// \brief	Gets a property int. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultString	Default string. 
///
/// \return	The property int. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTheme::GetPropertyInt(const std::string& component, const std::string& id, const std::string& defaultString)
{
	int result;

	std::string text = GetPropertyString(component, id);
	result = atoi(text.c_str());

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CTheme::GetPropertyString(const std::string& component, const std::string& id,
/// const std::string& defaultString)
///
/// \brief	Gets a property string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	component		The component. 
/// \param	id				The identifier. 
/// \param	defaultString	Default string. 
///
/// \return	The property string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CTheme::GetPropertyString(const std::string& component, const std::string& id,
									  const std::string& defaultString)
{
	std::string propertyName;
	propertyName.append(component);
	propertyName.append(id);
	// Search in the Cache
	if(m_mPropertyCache.find(propertyName) != m_mPropertyCache.end())
	{
		return m_mPropertyCache[propertyName];
	}
	std::string text;	
	std::string	dirINI;
	char		temp[MAX_PATH];
	
	dirINI = m_strThemePath;
	dirINI.append("manifest.ini");

	GetPrivateProfileString(component.c_str(), id.c_str(), defaultString.c_str(), temp, sizeof(temp), dirINI.c_str());
	text.append(temp);

	// Put into Cache
	m_mPropertyCache[propertyName] = text;
	return text;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CPicture* CTheme::GetPicture(int id)
///
/// \brief	Gets a picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
///
/// \return	null if it fails, else the picture. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CPicture* CTheme::GetPicture(int id)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	SallyAPI::GUI::CPicture* result = m_mGUICache[id];
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTheme::AddPicture(const std::string& filename, int id, int graphicId,
/// bool createMipMaps)
///
/// \brief	Adds a picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	filename		Filename of the file. 
/// \param	id				The identifier. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	createMipMaps	true to create mip maps. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTheme::AddPicture(const std::string& filename, int id, int graphicId, bool createMipMaps)
{
	AddPicture(filename, id + graphicId, createMipMaps);

	SallyAPI::System::CAutoLock lock(&m_Lock);
	m_mAdvancedImages[id + graphicId] = filename;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTheme::AddPicture(const std::string& filename, int id, bool createMipMaps)
///
/// \brief	Adds a picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	filename		Filename of the file. 
/// \param	id				The identifier. 
/// \param	createMipMaps	true to create mip maps. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTheme::AddPicture(const std::string& filename, int id, bool createMipMaps)
{
	std::string guiPath;
	SallyAPI::GUI::CPicture* pictureTemp;

	guiPath.append(m_strThemePath);
	guiPath.append(filename);

	pictureTemp = new SallyAPI::GUI::CPicture();
	if (pictureTemp->LoadTexture(guiPath, createMipMaps) == false)
	{
		delete pictureTemp;
		return;
	}

	SallyAPI::System::CAutoLock lock(&m_Lock);
	m_mGUICache[id] = pictureTemp;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTheme::AddPicturePlugin(const std::string& pluginPath,
/// const std::string& pluginName, const std::string& filename, bool createMipMaps, int id)
///
/// \brief	Adds a picture plugin. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	pluginPath		Full pathname of the plugin file. 
/// \param	pluginName		Name of the plugin. 
/// \param	filename		Filename of the file. 
/// \param	createMipMaps	true to create mip maps. 
/// \param	id				The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTheme::AddPicturePlugin(const std::string& pluginPath, const std::string& pluginName,
							  const std::string& filename, bool createMipMaps, int id)
{
	std::string guiPath;
	SallyAPI::GUI::CPicture* pictureTemp;

	guiPath.append(m_strThemePath);
	guiPath.append("applications\\");
	guiPath.append(pluginName);
	guiPath.append("\\");
	guiPath.append(filename);

	pictureTemp = new SallyAPI::GUI::CPicture();
	if (!pictureTemp->LoadTexture(guiPath, createMipMaps))
	{
		// Picture was not found
		guiPath.clear();
		guiPath.append(pluginPath);
		guiPath.append("pictures\\");
		guiPath.append(filename);
		if (!pictureTemp->LoadTexture(guiPath))
		{
			delete pictureTemp;
			return;
		}
	}
	SallyAPI::Config::CThemePluginImage* themePluginImage = new SallyAPI::Config::CThemePluginImage(pluginPath, pluginName, filename, createMipMaps);

	SallyAPI::System::CAutoLock lock(&m_Lock);
	m_mGUICache[id] = pictureTemp;
	m_mPluginImages[id] = themePluginImage;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTheme::RemovePicture(int id)
///
/// \brief	Removes the picture plugin described by id. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTheme::RemovePicture(int id)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	SallyAPI::GUI::CPicture* pictureTemp = m_mGUICache[id];
	SallyAPI::Config::CThemePluginImage* themePluginImage = m_mPluginImages[id];

	m_mGUICache.erase(id);
	m_mPluginImages.erase(id);

	SafeDelete(pictureTemp);
	SafeDelete(themePluginImage);
}