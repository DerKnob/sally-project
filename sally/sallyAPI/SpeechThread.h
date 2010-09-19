////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SpeechThread.h
///
/// \brief	Declares the speech thread class. 
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
#include "Window.h"
#include "Speech.h"
#include "Thread.h"

namespace SallyAPI
{
	namespace Speech
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CSpeechThread
		///
		/// \brief	Speech thread. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CSpeechThread :
			public SallyAPI::System::CThread
		{
		private:
			CRITICAL_SECTION			m_critSectLockSpeech;
			SallyAPI::GUI::CWindow*		m_pMainWindow;
			SallyAPI::Speech::CSpeech**	m_ppSpeech;
			std::vector<std::string>	m_vSeechList;
			std::map<std::string, bool>	m_vSeechListImportand;

			virtual void RunEx();
		public:
			CSpeechThread();
			virtual ~CSpeechThread();

			void SetValue(SallyAPI::Speech::CSpeech** speech, SallyAPI::GUI::CWindow* mainWindow);
			void Speak(const std::string& text, bool isImportand);
		};
	}
}
