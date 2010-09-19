////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SpeechThread.cpp
///
/// \brief	Implements the speech thread class. 
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

#include "SpeechThread.h"

using namespace SallyAPI::Speech;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSpeechThread::CSpeechThread()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSpeechThread::CSpeechThread()
{
	m_ppSpeech = NULL;
	InitializeCriticalSection(&m_critSectLockSpeech);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSpeechThread::~CSpeechThread()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSpeechThread::~CSpeechThread()
{
	DeleteCriticalSection(&m_critSectLockSpeech);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechThread::Speak(const std::string& text, bool isImportand)
///
/// \brief	Speaks. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	text		The text. 
/// \param	isImportand	true if is importand. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechThread::Speak(const std::string& text, bool isImportand)
{
	EnterCriticalSection(&m_critSectLockSpeech);
	m_vSeechList.push_back(text);
	if (isImportand)
		m_vSeechListImportand[text] = true;
	LeaveCriticalSection(&m_critSectLockSpeech);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechThread::SetValue(SallyAPI::Speech::CSpeech** speech,
/// SallyAPI::GUI::CWindow* mainWindow)
///
/// \brief	Sets a value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	speech		If non-null, the speech. 
/// \param [in,out]	mainWindow	If non-null, the main window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechThread::SetValue(SallyAPI::Speech::CSpeech** speech, SallyAPI::GUI::CWindow* mainWindow)
{
	m_ppSpeech = speech;
	m_pMainWindow = mainWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSpeechThread::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSpeechThread::RunEx()
{
	if (m_ppSpeech == NULL)
		return;
	if ((*m_ppSpeech) == NULL)
		return;

	EnterCriticalSection(&m_critSectLockSpeech);
	std::vector<std::string>::iterator iter = m_vSeechList.begin();

	while ((iter != m_vSeechList.end()) && (!m_bPleaseStop))
	{
		std::string temp = *iter;
		m_vSeechList.erase(iter);
		LeaveCriticalSection(&m_critSectLockSpeech);

		if (m_vSeechListImportand[temp])
			m_pMainWindow->SendMessageToParent(0, 0, MS_SALLY_MUTE_SOUND);

		(*m_ppSpeech)->Speak(temp);

		if (m_vSeechListImportand[temp])
			m_pMainWindow->SendMessageToParent(0, 0, MS_SALLY_UNMUTE_SOUND);

		m_vSeechListImportand.erase(temp);

		Sleep(100);

		EnterCriticalSection(&m_critSectLockSpeech);
		iter = m_vSeechList.begin();
	}
	LeaveCriticalSection(&m_critSectLockSpeech);
}