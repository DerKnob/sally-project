////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FontManager.cpp
///
/// \brief	Implements the font manager class. 
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

#include "FontManager.h"

using namespace SallyAPI::Core;

SallyAPI::Core::CFontManager*		CFontManager::m_pObject = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFontManager::CFontManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFontManager::CFontManager()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFontManager::~CFontManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFontManager::~CFontManager()
{
	std::map<std::string, SallyAPI::Core::CFont*>::iterator iter;

	iter = m_Fonts.begin();

	while (iter != m_Fonts.end())
	{
		delete(iter->second);
		m_Fonts.erase(iter);

		iter = m_Fonts.begin();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFontManager::Reload()
///
/// \brief	Reloads this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFontManager::Reload()
{
	SallyAPI::Core::CFontManager* newFontManager = new SallyAPI::Core::CFontManager();

	// Now switch
	SallyAPI::Core::CFontManager* temp = m_pObject;

	m_pObject = newFontManager;
	Sleep(200);
	SafeDelete(temp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFontManager* CFontManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CFontManager* CFontManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Core::CFontManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFontManager::LoadFont(const std::string& fontName)
///
/// \brief	Loads a font. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fontName	Name of the font. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFontManager::LoadFont(const std::string& fontName)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	std::string fontFace = theme->GetPropertyString(fontName.c_str(), "face", "Arial");
	int fontSize = theme->GetPropertyInt(fontName.c_str(), "size", "10");
	bool fontBold = theme->GetPropertyInt(fontName.c_str(), "bold", "0")?true:false;
	bool fontItalic = theme->GetPropertyInt(fontName.c_str(), "italic", "0")?true:false;

	SallyAPI::Core::CFont* font = new SallyAPI::Core::CFont(fontFace, fontSize, fontBold, fontItalic, false);

	font->SetColor(D3DCOLOR_XRGB(theme->GetPropertyInt(fontName.c_str(), "r"),
		theme->GetPropertyInt(fontName.c_str(), "g"),
		theme->GetPropertyInt(fontName.c_str(), "b")));

	m_Fonts[fontName] = font;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Core::CFont* CFontManager::GetFontByName(const std::string& fontName)
///
/// \brief	Gets a font by name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fontName	Name of the font. 
///
/// \return	null if it fails, else the font by name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Core::CFont* CFontManager::GetFontByName(const std::string& fontName)
{
	// if the font is not loaded, we use the default one
	if (m_Fonts[fontName] == NULL)
	{
		LoadFont(fontName);
		return m_Fonts[fontName];
	}
	return m_Fonts[fontName];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFontManager::RemoveFontByName(const std::string& fontName)
///
/// \brief	Removes the font by name described by fontName. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fontName	Name of the font. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFontManager::RemoveFontByName(const std::string& fontName)
{
	std::map<std::string, SallyAPI::Core::CFont*>::iterator iter;

	SallyAPI::Core::CFont* temp;
	iter = m_Fonts.begin();

	while (iter != m_Fonts.end())
	{
		if (iter->first.compare(fontName) == 0)
		{
			temp = iter->second;

			delete(iter->second);
			m_Fonts.erase(iter);
			return true;
		}
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFontManager::RemoveFontByFont(SallyAPI::Core::CFont* font)
///
/// \brief	Removes the font by font described by font. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	font	If non-null, the font. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFontManager::RemoveFontByFont(SallyAPI::Core::CFont* font)
{
	std::map<std::string, SallyAPI::Core::CFont*>::iterator iter;

	SallyAPI::Core::CFont* temp;
	iter = m_Fonts.begin();

	while (iter != m_Fonts.end())
	{
		temp = iter->second;

		if (temp == font)
		{
			delete(iter->second);
			m_Fonts.erase(iter);
			return true;
		}
		++iter;
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFontManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFontManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFontManager::OnLostDevice()
///
/// \brief	Executes the lost device action. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFontManager::OnLostDevice()
{
	std::map<std::string, SallyAPI::Core::CFont*>::iterator iterFont;
	SallyAPI::Core::CFont* font;

	for(iterFont = m_Fonts.begin(); iterFont != m_Fonts.end(); ++iterFont)
	{
		font = iterFont->second;
		HRESULT hr = font->OnLostDevice();
		if (hr != S_OK)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error("CFontManager::OnLostDevice() font->OnLostDevice()");
			logger->Error(hr);
			return false;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFontManager::OnResetDevice()
///
/// \brief	Executes the reset device action. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFontManager::OnResetDevice()
{
	std::map<std::string, SallyAPI::Core::CFont*>::iterator iterFont;
	SallyAPI::Core::CFont* font;

	for(iterFont = m_Fonts.begin(); iterFont != m_Fonts.end(); ++iterFont)
	{
		font = iterFont->second;
		HRESULT hr = font->OnResetDevice();
		if (hr != S_OK)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error("CFontManager::OnLostDevice() font->OnLostDevice()");
			logger->Error(hr);
			return false;
		}
	}
	return true;
}