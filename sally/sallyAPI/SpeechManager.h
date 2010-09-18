////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SpeechManager.h
///
/// \brief	Declares the speech manager class. 
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
#include "define.h"
#include "Window.h"
#include "Speech.h"
#include "SpeechManager.h"
#include "Config.h"
#include "SpeechThread.h"

namespace SallyAPI
{
	namespace Speech
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CSpeechOutputManager
		///
		/// \brief	Manager for speech outputs. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CSpeechOutputManager
		{
		private:
			static SallyAPI::GUI::CWindow*					m_pMainWindow;
			static SallyAPI::Speech::CSpeechOutputManager*	m_pObject;
			static SallyAPI::Speech::CSpeech*					m_pSpeech;
			static bool										m_bActive;

			SallyAPI::Speech::CSpeechThread					m_pSpeechThread;

			CSpeechOutputManager();
			~CSpeechOutputManager();
		public:
			static SallyAPI::Speech::CSpeechOutputManager*		GetInstance();
			static void	DeleteInstance();

			static void Activate();
			static void Deactivate();

			static void	SetValues(SallyAPI::GUI::CWindow* mainWindow);

			void	Stop();
			void	Speak(const std::string& say, bool isImportand = false);
			void	Speak(const std::string& say, bool isImportand, const char* first,...);
		};
	}
}