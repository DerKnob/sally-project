////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Config.h
///
/// \brief	Declares the configuration class. 
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

#pragma once
#include "Define.h"
#include <shlobj.h>
#include "Theme.h"
#include "LanguageManager.h"
#include "KeyboardLayout.h"
#include "SmartKeyboard.h"
#include "Option.h"
#include "SmartThreadPool.h"

namespace SallyAPI
{
	namespace Config
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CConfig
		///
		/// \brief	Configuration. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CConfig
		{
		private:
			static SallyAPI::Config::CConfig*	m_pObject;

			SallyAPI::System::COption*			m_pOption;
			SallyAPI::Config::CTheme*			m_pTheme;
			SallyAPI::Config::CLanguageManager*	m_pLanguageLocalization;
			SallyAPI::Config::CLanguageManager*	m_pSpeechOutputLocalization;
			SallyAPI::Config::CLanguageManager*	m_pSpeechInputLocalization;
			SallyAPI::Config::CKeyboardLayout*	m_pKeyboardLayout;
			SallyAPI::Config::CSmartKeyboard*	m_pSmartKeyboard;

			CConfig();
			~CConfig();
		public:
			static SallyAPI::Config::CConfig*	GetInstance();
			static void							DeleteInstance();

			static std::string					GetDefaultLanguage();

			SallyAPI::System::COption*			GetOption();
			CTheme*								GetTheme();
			SallyAPI::Config::CLanguageManager*	GetLanguageLocalization();
			SallyAPI::Config::CLanguageManager*	GetSpeechOutputLocalization();
			SallyAPI::Config::CLanguageManager*	GetSpeechInputLocalization();
			SallyAPI::Config::CKeyboardLayout*	GetKeyboardLayout();
			SallyAPI::Config::CSmartKeyboard*	GetSmartKeyboard();
			
			void		ReloadTheme();
			void		ReloadLanguage();
			void		ReloadOutputLanguage();
			void		ReloadInputLanguage();
			void		ReloadKeyboard();
		};
	}
}