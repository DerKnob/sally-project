////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\WindowMessage.cpp
///
/// \brief	Implements the window message class. 
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

#include "WindowMessage.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWindowMessage::CWindowMessage()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CWindowMessage::CWindowMessage()
:m_iMessageId(0), m_iReporterId(0), m_pReporter(NULL)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWindowMessage::CWindowMessage(int messageId, int repoterID,
/// SallyAPI::GUI::CGUIBaseObject* reporter)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	messageId			Identifier for the message. 
/// \param	repoterID			Identifier for the repoter. 
/// \param [in,out]	reporter	If non-null, the reporter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CWindowMessage::CWindowMessage(int messageId, int repoterID, SallyAPI::GUI::CGUIBaseObject* reporter)
:m_iMessageId(messageId), m_iReporterId(repoterID), m_pReporter(reporter)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CWindowMessage::~CWindowMessage()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CWindowMessage::~CWindowMessage()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CWindowMessage::GetMessageId()
///
/// \brief	Gets the message identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The message identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CWindowMessage::GetMessageId()
{
	return m_iMessageId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CWindowMessage::GetReporterId()
///
/// \brief	Gets the reporter identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The reporter identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CWindowMessage::GetReporterId()
{
	return m_iReporterId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CGUIBaseObject* CWindowMessage::GetReporter()
///
/// \brief	Gets the reporter. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the reporter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CGUIBaseObject* CWindowMessage::GetReporter()
{
	return m_pReporter;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindowMessage::SetMessageId(int message)
///
/// \brief	Sets a message identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	message	The message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindowMessage::SetMessageId(int message)
{
	m_iMessageId = message;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindowMessage::SetReporterId(int repoterId)
///
/// \brief	Sets a reporter identifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	repoterId	Identifier for the repoter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindowMessage::SetReporterId(int repoterId)
{
	m_iReporterId = repoterId;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CWindowMessage::SetReporter(SallyAPI::GUI::CGUIBaseObject* repoter)
///
/// \brief	Sets a reporter. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	repoter	If non-null, the repoter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CWindowMessage::SetReporter(SallyAPI::GUI::CGUIBaseObject* repoter)
{
	m_pReporter = repoter;
}