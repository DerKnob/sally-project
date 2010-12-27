////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Config.cpp
///
/// \brief	Implements the configuration class. 
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

#include "Config.h"

#include "ReloadThemeImage.h"
#include "FacebookManager.h"

using namespace SallyAPI::Config;

SallyAPI::Config::CConfig*		SallyAPI::Config::CConfig::m_pObject = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CConfig::CConfig()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CConfig::CConfig()
{
	std::string option = SallyAPI::Core::CGame::GetMediaFolder();
	option.append("option.ini");

	m_pOption = new SallyAPI::System::COption(option);
	std::string defaultLang = SallyAPI::Config::CConfig::GetDefaultLanguage();

	// theme
	{
		std::string name = m_pOption->GetPropertyString("sally", "theme", "de.der-knob.sally.theme.clean");

		std::string manifest = SallyAPI::System::SystemHelper::GetModulePath();

		manifest.append("themes\\");
		manifest.append(name);
		manifest.append("\\manifest.ini");

		if (!SallyAPI::File::FileHelper::FileExists(manifest))
		{
			// if the old theme does not exist anymore ... fallback to the default theme
			name = "de.der-knob.sally.theme.clean";
			m_pOption->SetPropertyString("sally", "theme", name);
		}
		m_pTheme = new CTheme(name);
	}

	// Keyboard
	{
		std::string defaultLangValue = "sally-project.keyboard.";
		defaultLangValue.append(defaultLang);

		std::string name = m_pOption->GetPropertyString("sally", "keyboardLayout", defaultLangValue);

		std::string manifest = SallyAPI::System::SystemHelper::GetModulePath();

		manifest.append("keyboards\\");
		manifest.append(name);
		manifest.append("\\manifest.ini");

		if (!SallyAPI::File::FileHelper::FileExists(manifest))
		{
			// if the old theme does not exist anymore ... fallback to the default theme
			name = "sally-project.keyboard.en";
			m_pOption->SetPropertyString("sally", "keyboardLayout", name);
		}
		m_pKeyboardLayout = new SallyAPI::Config::CKeyboardLayout(m_pOption->GetPropertyString("sally", "keyboardLayout", defaultLangValue));
		m_pSmartKeyboard = new SallyAPI::Config::CSmartKeyboard(m_pOption->GetPropertyString("sally", "keyboardLayout", defaultLangValue));
	}

	// GUI Language
	{
		std::string defaultLangValue = "sally-project.language.";
		defaultLangValue.append(defaultLang);

		std::string name = m_pOption->GetPropertyString("sally", "language", defaultLangValue);

		std::string manifest = SallyAPI::System::SystemHelper::GetModulePath();

		manifest.append("languages\\");
		manifest.append(name);
		manifest.append("\\manifest.ini");

		if (!SallyAPI::File::FileHelper::FileExists(manifest))
		{
			// if the old theme does not exist anymore ... fallback to the default theme
			name = "sally-project.language.en";
			m_pOption->SetPropertyString("sally", "language", name);
		}
		m_pLanguageLocalization = new SallyAPI::Config::CLanguageManager(m_pOption->GetPropertyString("sally", "language", defaultLangValue), "lang");
	}

	// Speech Output
	{
		std::string defaultLangValue = "sally-project.language.";
		defaultLangValue.append(defaultLang);

		std::string name = m_pOption->GetPropertyString("sally", "speechOutput", defaultLangValue);

		std::string manifest = SallyAPI::System::SystemHelper::GetModulePath();

		manifest.append("languages\\");
		manifest.append(name);
		manifest.append("\\manifest.ini");

		if (!SallyAPI::File::FileHelper::FileExists(manifest))
		{
			// if the old theme does not exist anymore ... fallback to the default theme
			name = "sally-project.language.en";
			m_pOption->SetPropertyString("sally", "speechOutput", name);
		}
		m_pSpeechOutputLocalization = new SallyAPI::Config::CLanguageManager(m_pOption->GetPropertyString("sally", "speechOutput", defaultLangValue), "speechOutput");
	}

	// Speech Input
	{
		std::string defaultLangValue = "sally-project.language.";
		defaultLangValue.append(defaultLang);

		std::string name = m_pOption->GetPropertyString("sally", "speechInput", defaultLangValue);

		std::string manifest = SallyAPI::System::SystemHelper::GetModulePath();

		manifest.append("languages\\");
		manifest.append(name);
		manifest.append("\\manifest.ini");

		if (!SallyAPI::File::FileHelper::FileExists(manifest))
		{
			// if the old theme does not exist anymore ... fallback to the default theme
			name = "sally-project.language.en";
			m_pOption->SetPropertyString("sally", "speechInput", name);
		}
		m_pSpeechInputLocalization = new SallyAPI::Config::CLanguageManager(m_pOption->GetPropertyString("sally", "speechInput", defaultLangValue), "speechInput");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CConfig::~CConfig()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CConfig::~CConfig()
{
	SafeDelete(m_pOption);
	SafeDelete(m_pTheme);
	SafeDelete(m_pLanguageLocalization);
	SafeDelete(m_pSpeechOutputLocalization);
	SafeDelete(m_pSpeechInputLocalization);
	SafeDelete(m_pKeyboardLayout);
	SafeDelete(m_pSmartKeyboard);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CConfig::GetDefaultLanguage()
///
/// \brief	Gets the default language. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The default language. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CConfig::GetDefaultLanguage()
{
	std::string defaultLang = "en";
	if ( PRIMARYLANGID( GetSystemDefaultLangID() ) == LANG_GERMAN )
	{
		defaultLang = "de";
	}
	/*
	else if ( PRIMARYLANGID( GetSystemDefaultLangID() ) == LANG_FRENCH )
	{
		defaultLang = "fr";
	}
	else if ( PRIMARYLANGID( GetSystemDefaultLangID() ) == LANG_SPANISH )
	{
		defaultLang = "es";
	}
	*/
	return defaultLang;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Config::CConfig* CConfig::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Config::CConfig* CConfig::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Config::CConfig();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::System::COption* CConfig::GetOption()
///
/// \brief	Gets the option. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the option. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::System::COption* CConfig::GetOption()
{
	return m_pOption;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Config::CTheme* CConfig::GetTheme()
///
/// \brief	Gets the theme. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the theme. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Config::CTheme* CConfig::GetTheme()
{
	return m_pTheme;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Config::CLanguageManager* CConfig::GetLanguageLocalization()
///
/// \brief	Gets the language localization. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the language localization. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Config::CLanguageManager* CConfig::GetLanguageLocalization()
{
	return m_pLanguageLocalization;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Config::CLanguageManager* CConfig::GetSpeechOutputLocalization()
///
/// \brief	Gets the speech output localization. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the speech output localization. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Config::CLanguageManager* CConfig::GetSpeechOutputLocalization()
{
	return m_pSpeechOutputLocalization;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Config::CLanguageManager* CConfig::GetSpeechInputLocalization()
///
/// \brief	Gets the speech input localization. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the speech input localization. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Config::CLanguageManager* CConfig::GetSpeechInputLocalization()
{
	return m_pSpeechInputLocalization;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Config::CKeyboardLayout* CConfig::GetKeyboardLayout()
///
/// \brief	Gets the keyboard layout. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the keyboard layout. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Config::CKeyboardLayout* CConfig::GetKeyboardLayout()
{
	return m_pKeyboardLayout;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Config::CSmartKeyboard* CConfig::GetSmartKeyboard()
///
/// \brief	Gets the smart keyboard. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the smart keyboard. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Config::CSmartKeyboard* CConfig::GetSmartKeyboard()
{
	return m_pSmartKeyboard;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfig::DeleteInstance()
///
/// \brief	Deletes the config instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfig::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfig::ReloadLanguage()
///
/// \brief	Reloads the language. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfig::ReloadLanguage()
{
	std::string defaultLang = SallyAPI::Config::CConfig::GetDefaultLanguage();

	SallyAPI::Config::CLanguageManager* oldLangManager = m_pLanguageLocalization;
	SallyAPI::Config::CLanguageManager* newLangManager = new CLanguageManager(m_pOption->GetPropertyString("sally", "language", defaultLang), "lang");

	std::vector<std::string> langFileDirs = oldLangManager->GetLangFileDirs();
	std::vector<std::string>::iterator iterLangFileDirs = langFileDirs.begin();

	while (iterLangFileDirs != langFileDirs.end())
	{
		newLangManager->AddLangFileDir(*iterLangFileDirs);
		++iterLangFileDirs;
	}

	m_pLanguageLocalization = newLangManager;

	Sleep(500);
	SafeDelete(oldLangManager);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfig::ReloadTheme()
///
/// \brief	Reloads the theme. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfig::ReloadTheme()
{
	SallyAPI::System::CSmartThreadPool loadTheme;

	CTheme* oldTheme = m_pTheme;
	CTheme* newTheme = new CTheme(m_pOption->GetPropertyString("sally", "theme"));

	// Images
	std::map<int, std::string>& advancedImages = oldTheme->GetAdvancedImages();
	std::map<int, std::string>::iterator iter = advancedImages.begin();

	while (iter != advancedImages.end())
	{
		loadTheme.AddThread(new SallyAPI::Config::CReloadThemeImage(newTheme, iter->second, iter->first));
		++iter;
	}

	std::map<int, SallyAPI::Config::CThemePluginImage*>& pluginImages = oldTheme->GetPluginImages();
	std::map<int, SallyAPI::Config::CThemePluginImage*>::iterator iterPlugin = pluginImages.begin();

	while (iterPlugin != pluginImages.end())
	{
		SallyAPI::Config::CThemePluginImage* themePluginImage = iterPlugin->second;

		if (themePluginImage != NULL)
		{
			loadTheme.AddThread(new SallyAPI::Config::CReloadThemeImage(newTheme, themePluginImage, iterPlugin->first));
		}
		++iterPlugin;
	}

	loadTheme.Start();
	while (loadTheme.GetStatus() != SallyAPI::System::THREAD_STOPPED)
	{
		Sleep(10);
	}

	m_pTheme = newTheme;

	// load facebook images
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	facebookManager->ReloadAllFacebookUserImages();

	Sleep(500);
	SafeDelete(oldTheme);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfig::ReloadInputLanguage()
///
/// \brief	Reloads the input language. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfig::ReloadInputLanguage()
{
	std::string defaultLang =  SallyAPI::Config::CConfig::GetDefaultLanguage();

	SallyAPI::Config::CLanguageManager* oldLangManager = m_pSpeechInputLocalization;
	SallyAPI::Config::CLanguageManager* newLangManager = new SallyAPI::Config::CLanguageManager(m_pOption->GetPropertyString("sally", "speechInput", defaultLang), "speechInput");

	std::vector<std::string> langFileDirs = oldLangManager->GetLangFileDirs();
	std::vector<std::string>::iterator iterLangFileDirs = langFileDirs.begin();

	while (iterLangFileDirs != langFileDirs.end())
	{
		newLangManager->AddLangFileDir(*iterLangFileDirs);
		++iterLangFileDirs;
	}

	m_pSpeechInputLocalization = newLangManager;

	Sleep(500);
	SafeDelete(oldLangManager);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfig::ReloadOutputLanguage()
///
/// \brief	Reloads the output language. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfig::ReloadOutputLanguage()
{
	std::string defaultLang =  SallyAPI::Config::CConfig::GetDefaultLanguage();

	SallyAPI::Config::CLanguageManager* oldLangManager = m_pSpeechOutputLocalization;
	SallyAPI::Config::CLanguageManager* newLangManager = new SallyAPI::Config::CLanguageManager(m_pOption->GetPropertyString("sally", "speechOutput", defaultLang), "speechOutput");

	std::vector<std::string> langFileDirs = oldLangManager->GetLangFileDirs();
	std::vector<std::string>::iterator iterLangFileDirs = langFileDirs.begin();

	while (iterLangFileDirs != langFileDirs.end())
	{
		newLangManager->AddLangFileDir(*iterLangFileDirs);
		++iterLangFileDirs;
	}

	m_pSpeechOutputLocalization = newLangManager;

	Sleep(500);
	SafeDelete(oldLangManager);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CConfig::ReloadKeyboard()
///
/// \brief	Reloads the keyboard. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CConfig::ReloadKeyboard()
{
	std::string defaultLang =  SallyAPI::Config::CConfig::GetDefaultLanguage();

	CKeyboardLayout* oldKeyboardLayout = m_pKeyboardLayout;
	CSmartKeyboard* oldSmartKeyboard = m_pSmartKeyboard;

	CKeyboardLayout* newKeyboardLayout = new CKeyboardLayout(m_pOption->GetPropertyString("sally", "keyboardLayout", defaultLang));
	CSmartKeyboard* newSmartKeyboard = new CSmartKeyboard(m_pOption->GetPropertyString("sally", "keyboardLayout", defaultLang));

	m_pKeyboardLayout = newKeyboardLayout;
	m_pSmartKeyboard = newSmartKeyboard;

	Sleep(500);
	SafeDelete(oldKeyboardLayout);
	SafeDelete(oldSmartKeyboard);
}