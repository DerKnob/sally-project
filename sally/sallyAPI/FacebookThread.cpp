////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FacebookThread.cpp
///
/// \brief	Implements the facebook thread class. 
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

#include "FacebookThread.h"

using namespace SallyAPI::Facebook;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFacebookThread::CFacebookThread()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFacebookThread::CFacebookThread()
{
	InitializeCriticalSection(&m_critSectLockRequest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFacebookThread::~CFacebookThread()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFacebookThread::~CFacebookThread()
{
	DeleteCriticalSection(&m_critSectLockRequest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookThread::Request(const std::string& server, int port,
/// const std::string& request, const std::string& post)
///
/// \brief	Requests. 
///
/// \author	Christian Knobloch
/// \date	24.12.2010
///
/// \param	server	The server. 
/// \param	port	The port. 
/// \param	request	The request. 
/// \param	post	The post. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookThread::Request(const std::string& server, int port, const std::string& request, 
							  const std::string& post)
{
	std::string todo = server;
	todo.append("###");
	todo.append(SallyAPI::String::StringHelper::ConvertToString(port));
	todo.append("###");
	todo.append(request);
	todo.append("###");
	todo.append(post);

	EnterCriticalSection(&m_critSectLockRequest);
	m_vRequestList.push_back(todo);
	LeaveCriticalSection(&m_critSectLockRequest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookThread::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookThread::RunEx()
{
	EnterCriticalSection(&m_critSectLockRequest);
	std::vector<std::string>::iterator iter = m_vRequestList.begin();

	while ((iter != m_vRequestList.end()) && (!m_bPleaseStop))
	{
		std::string temp = *iter;
		m_vRequestList.erase(iter);
		LeaveCriticalSection(&m_critSectLockRequest);

		// split the srting to get the server, port and request
		std::vector<std::string> parms = SallyAPI::String::StringHelper::TokenizeString(temp, "###");

		if (parms.size() != 4)
			continue;

		DWORD iFlag = INTERNET_FLAG_RELOAD;

		// do we have a https request?
		if (parms[1].compare("443") == 0)
			iFlag |= INTERNET_FLAG_SECURE;

		// make request
		std::string response;
		int byteRead = 0;

		std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
		std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
		SallyAPI::Network::NetworkHelper::GetHTTPText(parms[0],
			SallyAPI::String::StringHelper::ConvertToInt(parms[1]), parms[2],
			&byteRead, &response, proxy, proxyBypass, iFlag, 10, NULL, &parms[3]);

		Sleep(1);

		EnterCriticalSection(&m_critSectLockRequest);
		iter = m_vRequestList.begin();
	}
	LeaveCriticalSection(&m_critSectLockRequest);
}