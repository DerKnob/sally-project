////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\HttpSendRequestThread.cpp
///
/// \brief	Implements the http send request thread class. 
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

#include "HttpSendRequestThread.h"

using namespace SallyAPI::Network;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CHttpSendRequestThread::CHttpSendRequestThread()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CHttpSendRequestThread::CHttpSendRequestThread()
	:m_iErrorCode(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CHttpSendRequestThread::~CHttpSendRequestThread()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CHttpSendRequestThread::~CHttpSendRequestThread()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CHttpSendRequestThread::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CHttpSendRequestThread::RunEx()
{
	if (m_strPostData.length() == 0)
		HttpSendRequest(m_pHttpRequest, 0, 0, 0, 0);
	else
		HttpSendRequest(m_pHttpRequest, "Content-Type: application/x-www-form-urlencoded", -1, (LPVOID) m_strPostData.c_str(), strlen(m_strPostData.c_str()));

	m_iErrorCode = GetLastError();

	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CHttpSendRequestThread::SetValues(HINTERNET httpRequest)
///
/// \brief	Sets the values. 
///
/// \author	Christian Knobloch
/// \date	17.12.2010
///
/// \param	httpRequest	The http request. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CHttpSendRequestThread::SetValues(HINTERNET httpRequest)
{
	m_pHttpRequest = httpRequest;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CHttpSendRequestThread::SetValues(HINTERNET httpRequest, std::string postData)
///
/// \brief	Sets the values. 
///
/// \author	Christian Knobloch
/// \date	17.12.2010
///
/// \param	httpRequest	The http request. 
/// \param	postData	Information describing the post. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CHttpSendRequestThread::SetValues(HINTERNET httpRequest, const std::string& postData)
{
	m_pHttpRequest = httpRequest;
	m_strPostData = postData;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CHttpSendRequestThread::GetErrorCode()
///
/// \brief	Gets the error code. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \return	The error code. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CHttpSendRequestThread::GetErrorCode()
{
	return m_iErrorCode;
}