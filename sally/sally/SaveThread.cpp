////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SaveThread.cpp
///
/// \brief	Implements the save thread class. 
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

#include "SaveThread.h"

CSaveThread::CSaveThread()
{
	m_pWizardPanel = NULL;
	m_pParentControl = NULL;
	m_iMessage = 0;
}

CSaveThread::~CSaveThread()
{
}

void CSaveThread::RunEx()
{
	m_pWizardPanel->SaveConfig();

	// [SA-00455] Sally: FirstStartWizard problems
	// + "please wait" is hanging sometimes
	// the hide working is maybe fired to fast?
	Sleep(1000);

	m_pParentControl->SendMessageToParent(0, 0, m_iMessage);
}

void CSaveThread::SetValues(SallyAPI::GUI::CWizardPanel* wizardPanel, SallyAPI::GUI::CControl* appWindow, int message)
{
	m_pWizardPanel = wizardPanel;
	m_pParentControl = appWindow;
	m_iMessage = message;
}