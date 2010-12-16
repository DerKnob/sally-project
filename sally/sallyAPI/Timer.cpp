////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Timer.cpp
///
/// \brief	Implements the timer class. 
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

#include "Timer.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTimer::CTimer(int timeoutSecond, SallyAPI::GUI::CGUIBaseObject* reporterWindow,
/// int reporterId, int messageId) :SallyAPI::System::CThread(), m_iTimeoutSecond(timeoutSecond),
/// m_pReporterWindow(reporterWindow), m_iReporterId(reporterId), m_iMessageId(messageId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeoutSecond			The timeout second. 
/// \param [in,out]	reporterWindow	If non-null, the reporter window. 
/// \param	reporterId				Identifier for the reporter. 
/// \param	messageId				Identifier for the message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CTimer::CTimer(int timeoutSecond, SallyAPI::GUI::CGUIBaseObject* reporterWindow, int reporterId, int messageId)
	:SallyAPI::System::CThread(), m_iTimeoutSecond(timeoutSecond), m_pReporterWindow(reporterWindow),
	m_iReporterId(reporterId), m_iMessageId(messageId)
{
	Reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTimer::~CTimer()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTimer::~CTimer()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTimer::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTimer::RunEx()
{
	while (m_eStatus == SallyAPI::System::THREAD_RUNNING && m_bPleaseStop == false)
	{
		Sleep(1000); // sleep for 1 second
		if (m_bPleaseStop == false)
		{
			++m_iTimeoutElapse;
			if (m_iTimeoutElapse >= m_iTimeoutSecond)
			{
				m_iTimeoutElapse = 0;
				m_pReporterWindow->SendMessageToParent(m_pReporterWindow, m_iReporterId, m_iMessageId);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTimer::Reset()
///
/// \brief	Resets this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTimer::Reset()
{
	m_iTimeoutElapse = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTimer::ExecuteNow()
///
/// \brief	Executes the now operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTimer::ExecuteNow()
{
	m_iTimeoutElapse = m_iTimeoutSecond;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTimer::SetTimeout(int timeoutSecond)
///
/// \brief	Sets a timeout. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeoutSecond	The timeout second. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTimer::SetTimeout(int timeoutSecond)
{
	m_iTimeoutSecond = timeoutSecond;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTimer::GetTimeElapse()
///
/// \brief	Gets the time elapse. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The time elapse. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTimer::GetTimeElapse()
{
	return m_iTimeoutElapse;
}