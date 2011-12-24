////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ApplicationWindow.h
///
/// \brief	Declares the application Windows Form. 
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

#pragma once
#include "Define.h"
#include "Config.h"
#include "AppBase.h"
#include "WizardPanel.h"
#include "ParameterIntegerVector.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CApplicationWindow
		///
		/// \brief	The main class to inherit from when creating a sally application or screensaver
		///         (or both). 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CApplicationWindow :
			public SallyAPI::GUI::CAppBase
		{
		private:
			std::string						m_strAPIVersion;
		protected:
			std::string						m_strPluginPath;
			std::string						m_strAppName;
			std::string						m_strAuthor;
			std::string						m_strVersion;

			void			UnloadPicture(int id);
			void			LoadApplicationImage(const std::string& name, int id, bool scalable = false);
			virtual void	LoadConfig(SallyAPI::GUI::SendMessage::CParameterIntegerVector* messageParameter = NULL);
			virtual void	SaveConfig();
			virtual void	SallyConfigChanged();
			virtual void	MuteSound();
			virtual void	UnMuteSound();
		public:
			CApplicationWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
			virtual ~CApplicationWindow();

			std::string GetAPIVersion();
			std::string GetAppName();
			std::string GetAuthor();
			std::string GetVersion();
			std::string GetPluginPath();

			// Screensaver Functions
			virtual bool	ActivateScreensaver();
			virtual bool	DeactivateScreensaver();
			
			virtual bool	HasScreensaver();
			virtual bool	HasApplication();
			virtual bool	IsFacebookNeeded();
			virtual bool	IsGame();

			// Voice Input
			virtual	bool	VoiceCommand(const std::string& command);
			virtual	bool	VoiceCommandActive(const std::string& command);
			virtual	bool	VoiceCommandFocus(const std::string& command);

			virtual bool	SpecialKeyPressed(int key);

			virtual void	SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}