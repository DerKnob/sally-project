////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\PlaySound.cpp
///
/// \brief	Implements the play sound class. 
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

#include "PlaySound.h"

using namespace SallyAPI::Sound;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPlaySound::CPlaySound(SallyAPI::GUI::CControl* mainWindow, const std::string& soundFile)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	mainWindow	If non-null, the main window. 
/// \param	soundFile			The sound file. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CPlaySound::CPlaySound(SallyAPI::GUI::CControl* mainWindow, const std::string& soundFile)
	: m_pSoundFile(soundFile), m_pMainWindow(mainWindow)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPlaySound::CPlaySound(SallyAPI::GUI::CControl* mainWindow, DWORD freq, DWORD duration,
/// int repeat, int repeatSleepMilliseconds)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	mainWindow		If non-null, the main window. 
/// \param	freq					The freq. 
/// \param	duration				The duration. 
/// \param	repeat					The repeat. 
/// \param	repeatSleepMilliseconds	The repeat sleep in milliseconds. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CPlaySound::CPlaySound(SallyAPI::GUI::CControl* mainWindow, DWORD freq, DWORD duration, int repeat, int repeatSleepMilliseconds)
	: m_pMainWindow(mainWindow), m_dwFreq(freq), m_dwDuration(duration), m_iRepeat(repeat), m_iRepeatSleepMilliseconds(repeatSleepMilliseconds)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPlaySound::~CPlaySound ()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CPlaySound::~CPlaySound ()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPlaySound::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPlaySound::RunEx()
{
	m_pMainWindow->SendMessageToParent(0, 0, MS_SALLY_MUTE_SOUND);

	if (m_pSoundFile.length() != 0)
	{
		PlaySound(m_pSoundFile.c_str(), NULL, SND_FILENAME | SND_SYNC);
	}
	else
	{
		for (int i = 0; i < m_iRepeat; ++i)
		{
			Beep(m_dwFreq, m_dwDuration);
			if (i < m_iRepeat - 1)
			{
				Sleep(m_iRepeatSleepMilliseconds);
			}
		}
	}

	m_pMainWindow->SendMessageToParent(0, 0, MS_SALLY_UNMUTE_SOUND);
}