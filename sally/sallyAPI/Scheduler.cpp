////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Scheduler.cpp
///
/// \brief	Implements the scheduler class. 
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

#include "Scheduler.h"

using namespace SallyAPI::Scheduler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScheduler::CScheduler(SallyAPI::GUI::CAppBase* reporterWindow, int reporterId,
/// int messageId, const std::string& identifier, int runEveryDays)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporterWindow	If non-null, the reporter window. 
/// \param	reporterId				Identifier for the reporter. 
/// \param	messageId				Identifier for the message. 
/// \param	identifier				The identifier. 
/// \param	runEveryDays			The run every in days. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScheduler::CScheduler(SallyAPI::GUI::CAppBase* reporterWindow, int reporterId, int messageId,
					   const std::string& identifier, int runEveryDays)
	: m_pReporterWindow(reporterWindow), m_iReporterId(reporterId), m_iMessageId(messageId), m_bRunning(false),
	m_strIdentifier(identifier), m_iRunEveryDays(runEveryDays)
{
	ClearStartTime();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScheduler::~CScheduler()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CScheduler::~CScheduler()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScheduler::ClearStartTime()
///
/// \brief	Clears a start time. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScheduler::ClearStartTime()
{
	ZeroMemory(&m_stStartTime, sizeof(SYSTEMTIME));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CAppBase* CScheduler::GetReporterWindow()
///
/// \brief	Gets the reporter window. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the reporter window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CAppBase* CScheduler::GetReporterWindow()
{
	return m_pReporterWindow;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CScheduler::GetIdentifier()
///
/// \brief	Gets the identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CScheduler::GetIdentifier()
{
	return m_strIdentifier;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CScheduler::GetReporterId()
///
/// \brief	Gets the reporter identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The reporter identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CScheduler::GetReporterId()
{
	return m_iReporterId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CScheduler::GetMessageId()
///
/// \brief	Gets the message identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The message identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CScheduler::GetMessageId()
{
	return m_iMessageId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CScheduler::GetRunEveryDays()
///
/// \brief	Gets the run every days. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The run every days. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CScheduler::GetRunEveryDays()
{
	return m_iRunEveryDays;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SYSTEMTIME CScheduler::GetStartTime()
///
/// \brief	Gets the start time. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The start time. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SYSTEMTIME CScheduler::GetStartTime()
{
	return m_stStartTime;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScheduler::SetRunning(bool running)
///
/// \brief	Sets a running. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	running	true to running. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScheduler::SetRunning(bool running)
{
	m_bRunning = running;

	if (m_bRunning)
	{
		GetLocalTime(&m_stStartTime);
	}
	else
	{
		ClearStartTime();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScheduler::IsRunning()
///
/// \brief	Query if this object is running. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if running, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScheduler::IsRunning()
{
	return m_bRunning;
}