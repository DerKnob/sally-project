////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\CommunityThread.cpp
///
/// \brief	Implements the community thread class. 
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

#include "CommunityThread.h"

using namespace SallyAPI::Community;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCommunityThread::CCommunityThread()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCommunityThread::CCommunityThread()
{
	InitializeCriticalSection(&m_critSectLockRequest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCommunityThread::~CCommunityThread()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCommunityThread::~CCommunityThread()
{
	DeleteCriticalSection(&m_critSectLockRequest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCommunityThread::Request(const std::string& request)
///
/// \brief	Requests. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	request	The request. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommunityThread::Request(const std::string& request)
{
	EnterCriticalSection(&m_critSectLockRequest);
	m_vRequestList.push_back(request);

	LeaveCriticalSection(&m_critSectLockRequest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCommunityThread::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommunityThread::RunEx()
{
	EnterCriticalSection(&m_critSectLockRequest);
	std::vector<std::string>::iterator iter = m_vRequestList.begin();

	while ((iter != m_vRequestList.end()) && (!m_bPleaseStop))
	{
		std::string temp = *iter;
		m_vRequestList.erase(iter);
		LeaveCriticalSection(&m_critSectLockRequest);

		// make request
		std::string response;
		int byteRead = 0;

		std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
		std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
		SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT, temp, &byteRead, &response, proxy, proxyBypass);

		Sleep(1);

		EnterCriticalSection(&m_critSectLockRequest);
		iter = m_vRequestList.begin();
	}
	LeaveCriticalSection(&m_critSectLockRequest);
}