////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\UpdateProcessWatcher.cpp
///
/// \brief	Implements the update process watcher class. 
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

#include "UpdateProcessWatcher.h"

CUpdateProcessWatcher::CUpdateProcessWatcher(SallyAPI::GUI::CControl* parent, int messageId)
	:m_pParent(parent), m_iMessageId(messageId)
{
}

CUpdateProcessWatcher::~CUpdateProcessWatcher()
{
}

void CUpdateProcessWatcher::RunEx()
{
	std::string modulePath = "\"";
	modulePath.append(SallyAPI::System::SystemHelper::GetModulePath());
	modulePath.append("SallyUpdate.exe\"");

	STARTUPINFO            si; 
	PROCESS_INFORMATION    pi; 

	memset(&si, 0, sizeof(si)); 
	memset(&pi, 0, sizeof(pi)); 
	si.cb = sizeof( STARTUPINFO);

	if(CreateProcess(NULL, (LPSTR) modulePath.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi) == 0)
	{ 
		m_pParent->SendMessageToParent(m_pParent, 0, m_iMessageId);
		return;
	} 
	WaitForInputIdle(pi.hProcess, INFINITE); 
	do 
	{ 
		// whait for the ending of the process
	} 
	while ((WaitForSingleObject(pi.hProcess, 1000) == WAIT_TIMEOUT) && (!m_bPleaseStop));

	if (m_bPleaseStop)
		return;

	WaitForSingleObject(pi.hProcess, INFINITE); 
	CloseHandle(pi.hProcess);

	m_pParent->SendMessageToParent(m_pParent, 0, m_iMessageId);

	return;
}