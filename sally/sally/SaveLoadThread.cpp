////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SaveLoadThread.cpp
///
/// \brief	Implements the save load thread class. 
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
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SaveLoadThread.h"

CSaveLoadThread::CSaveLoadThread(void)
{
	m_pConfigPanel = NULL;
	m_pAppWindow = NULL;
	m_iMessage = 0;
	m_bSave = false;
}

CSaveLoadThread::~CSaveLoadThread(void)
{
}

void CSaveLoadThread::RunEx()
{
	if (m_bSave)
	{
		std::vector<int> changedValues = m_pConfigPanel->SaveConfig();
		SallyAPI::GUI::SendMessage::CParameterIntegerVector	integerVector(changedValues);
		m_pConfigPanel->SendMessageToParent(0, 0, MS_SALLY_APP_CONFIG_SAVED, &integerVector);
	}
	else
	{
		m_pConfigPanel->LoadConfig();
	}

	m_pAppWindow->SendMessageToParent(0, 0, m_iMessage);
}

void CSaveLoadThread::SetValues(SallyAPI::GUI::CConfigPanel* configPanel, SallyAPI::GUI::CApplicationWindow* appWindow, int message, bool save)
{
	m_pConfigPanel = configPanel;
	m_pAppWindow = appWindow;
	m_iMessage = message;
	m_bSave = save;
}