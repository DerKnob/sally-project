////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ApplicationWindow.cpp
///
/// \brief	Implements the application Windows Form. 
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

#include "ApplicationWindow.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CApplicationWindow::CApplicationWindow(SallyAPI::GUI::CGUIBaseObject* parent,
/// int graphicId, const std::string& pluginPath) :SallyAPI::GUI::CAppBase(parent, graphicId,
/// pluginPath), m_strPluginPath(pluginPath)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	pluginPath		Full pathname of the plugin file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CApplicationWindow::CApplicationWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
												 const std::string& pluginPath)
	:SallyAPI::GUI::CAppBase(parent, graphicId, pluginPath), m_strPluginPath(pluginPath)
{
	m_strAPIVersion = SALLY_API_VERSION;

	if (pluginPath.length() > 0)
	{
		std::string manifest = pluginPath;
		manifest.append("manifest.ini");

		m_strAppName = SallyAPI::System::COption::GetPropertyStringStatic("info", "name", "", manifest);
		m_strAuthor = SallyAPI::System::COption::GetPropertyStringStatic("info", "author", "", manifest);
		m_strVersion = SallyAPI::System::COption::GetPropertyStringStatic("info", "version", "", manifest);
	}

	if (m_strExplicitAppName.length() > 0)
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();

		SallyAPI::Config::CLanguageManager* languageLocalization = config->GetLanguageLocalization();
		SallyAPI::Config::CLanguageManager* speechOutputLocalization = config->GetSpeechOutputLocalization();
		SallyAPI::Config::CLanguageManager* speechInputLocalization = config->GetSpeechInputLocalization();

		// Load Application Picture
		LoadApplicationImage("app.png", 0, true);

		// Add the Language Directory
		languageLocalization->AddLangFileDir(m_strExplicitAppName);
		// Add the SpeechInput Directory
		speechOutputLocalization->AddLangFileDir(m_strExplicitAppName);
		// Add the SpeechOutput Directory
		speechInputLocalization->AddLangFileDir(m_strExplicitAppName);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CApplicationWindow::~CApplicationWindow()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CApplicationWindow::~CApplicationWindow()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::VoiceCommand(const std::string& command)
///
/// \brief	Voice command. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	command	The command. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::VoiceCommand(const std::string& command)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CApplicationWindow::GetAPIVersion()
///
/// \brief	Gets a pi version. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	a pi version. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CApplicationWindow::GetAPIVersion()
{
	return m_strAPIVersion;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CApplicationWindow::GetAppName()
///
/// \brief	Gets the application name. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The application name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CApplicationWindow::GetAppName()
{
	return m_strAppName;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CApplicationWindow::GetAuthor()
///
/// \brief	Gets the author. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The author. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CApplicationWindow::GetAuthor()
{
	return m_strAuthor;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CApplicationWindow::GetVersion()
///
/// \brief	Gets the version. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The version. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CApplicationWindow::GetVersion()
{
	return m_strVersion;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CApplicationWindow::GetPluginPath()
///
/// \brief	Gets the plugin path. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The plugin path. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CApplicationWindow::GetPluginPath()
{
	return m_strPluginPath;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::VoiceCommandActive(const std::string& command)
///
/// \brief	Voice command active. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	command	The command. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::VoiceCommandActive(const std::string& command)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::VoiceCommandFocus( const std::string& command )
///
/// \brief	Voice command focus. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	command	The command. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::VoiceCommandFocus( const std::string& command )
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::SpecialKeyPressed(int key)
///
/// \brief	Special key pressed. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	key	The key. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::SpecialKeyPressed(int key)
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::LoadConfig()
///
/// \brief	Is called when the configuration should be (re)loads. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::LoadConfig()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::SaveConfig()
///
/// \brief	Is called when the configuration should be saved. 
///
/// \author	Christian Knobloch
/// \date	25.05.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::SaveConfig()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::SallyConfigChanged()
///
/// \brief	The config for this application was changed.
///			Normally called from the the save action of the config panel.
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::SallyConfigChanged()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to childs. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_DEVICE_RESTORE_START:
		return;
	case MS_SALLY_DEVICE_RESTORE_END:
		return;
	}
	SallyAPI::GUI::CAppBase::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_APP_SAVE_CONFIG:
		SaveConfig();
		return;
	case MS_SALLY_APP_CONFIG_CHANGED:
		LoadConfig();
		return;
	case MS_SALLY_SALLY_CONFIG_CHANGED:
		SallyConfigChanged();
		return;
	case MS_SALLY_APP_MUTE_SOUND:
		MuteSound();
		return;
	case MS_SALLY_APP_UNMUTE_SOUND:
		UnMuteSound();
		return;
	case MS_SALLY_APP_LOAD_COMPLETE:
		return;
	case MS_SALLY_DEVICE_LOST:
		return;
	case MS_SALLY_DEVICE_RESTORE_START:
		return;
	case MS_SALLY_DEVICE_RESTORE_END:
		return;
	case MS_SALLY_FACEBOOK_CONFIG_CHANGED:
		return;
	}
	SallyAPI::GUI::CAppBase::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::LoadApplicationImage(const std::string& name, int id, bool scalable)
///
/// \brief	Loads a picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	name		The name. 
/// \param	id			The identifier. 
/// \param	scalable	true to scalable. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::LoadApplicationImage(const std::string& name, int id, bool scalable)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	theme->AddPicturePlugin(m_strPluginPath, m_strExplicitAppName, name, scalable, id + GetGraphicId());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::UnloadPicture(int id)
///
/// \brief	Unload picture. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	id	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::UnloadPicture(int id)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	theme->RemovePicture(id + GetGraphicId());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::MuteSound()
///
/// \brief	Mute sound. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::MuteSound()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CApplicationWindow::UnMuteSound()
///
/// \brief	Un mute sound. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CApplicationWindow::UnMuteSound()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::HasScreensaver()
///
/// \brief	Query if this object has screensaver. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if screensaver, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::HasScreensaver()
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::HasApplication()
///
/// \brief	Query if this object has application. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if application, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::HasApplication()
{
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::IsFacebookNeeded()
///
/// \brief	Query if this facebook connect need to be enabled. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
///
/// \return	true if facebook connect needed, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::IsFacebookNeeded()
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::IsGame()
///
/// \brief	Query if this object is game. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
///
/// \return	true if game, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::IsGame()
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::ActivateScreensaver()
///
/// \brief	Activates a screensaver. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::ActivateScreensaver()
{
	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CApplicationWindow::DeactivateScreensaver()
///
/// \brief	Deactivate screensaver. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CApplicationWindow::DeactivateScreensaver()
{
	return true;
}