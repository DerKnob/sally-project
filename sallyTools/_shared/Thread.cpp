////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\Thread.cpp
///
/// \brief	Implements the thread class. 
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

#include "Thread.h"

using namespace SallyAPI::System;

CThread::CThread()
{
	m_hThread = NULL;
	m_eStatus = THREAD_STOPPED;
	m_bPleaseStop = false;
}

CThread::~CThread()
{
	m_eStatus = THREAD_STOPPED;
}

DWORD WINAPI CThread::Thread(PVOID pvoid)
{
	CThread* parent = reinterpret_cast<CThread*>(pvoid);

	parent->Run();

	ExitThread(0);
}


ThreadStatus CThread::GetStatus()
{
	return m_eStatus;
}

void CThread::Start()
{
	if (m_hThread != NULL)
		return;

	m_eStatus = THREAD_RUNNING;
	m_hThread = ::CreateThread(NULL, NULL, CThread::Thread, this, 0, 0);
}

void CThread::Stop(bool force)
{
	if (m_eStatus == THREAD_STOPPED)
		return;

	m_bPleaseStop = true;

	if (force)
	{
		TerminateThread(m_hThread, 0);
		m_eStatus = THREAD_STOPPED;
		m_hThread = NULL;
		m_bPleaseStop = false;
	}
}

void CThread::WaitForStop(bool force)
{
	Stop(force);

	// now wait for the stop
	while (m_eStatus == THREAD_RUNNING)
	{
		Sleep(1);
	}
}

void CThread::Run()
{
	RunEx();

	m_eStatus = THREAD_STOPPED;
	m_hThread = NULL;
	m_bPleaseStop = false;
}