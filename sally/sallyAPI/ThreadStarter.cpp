////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ThreadStarter.cpp
///
/// \brief	Implements the thread starter class. 
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

#include "ThreadStarter.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CThreadStarter::CThreadStarter(SallyAPI::GUI::CGUIBaseObject* reporterWindow,
/// int reporterId, int messageId) :SallyAPI::System::CThread(),
/// m_pReporterWindow(reporterWindow), m_iReporterId(reporterId), m_iMessageId(messageId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporterWindow	If non-null, the reporter window. 
/// \param	reporterId				Identifier for the reporter. 
/// \param	messageId				Identifier for the message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CThreadStarter::CThreadStarter(SallyAPI::GUI::CGUIBaseObject* reporterWindow, int reporterId, int messageId)
	:SallyAPI::System::CThread(), m_pReporterWindow(reporterWindow), m_iReporterId(reporterId), m_iMessageId(messageId)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CThreadStarter::~CThreadStarter()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CThreadStarter::~CThreadStarter()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CThreadStarter::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CThreadStarter::RunEx()
{
	m_pReporterWindow->SendMessageToParent(m_pReporterWindow, m_iReporterId, m_iMessageId);
}