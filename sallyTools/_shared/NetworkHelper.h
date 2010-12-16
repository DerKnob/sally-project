////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\NetworkHelper.h
///
/// \brief	Declares the network helper class. 
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

#pragma once

#pragma comment( lib, "Wininet.lib" )

#include "Define.h"
#include <Wininet.h>
#include <string>
#include "HttpSendRequestThread.h"

namespace SallyAPI
{
	namespace Network
	{
		enum NETWORK_RETURN {SUCCESS, ERROR_PREPARE, ERROR_HTTP_TIMEOUT, ERROR_OPEN, ERROR_NOTHING_READ};

		class DLL_API_SALLY NetworkHelper
		{
		private:
			NetworkHelper();
			~NetworkHelper();
		public:
			static SallyAPI::Network::NETWORK_RETURN GetHTTPText(const std::string& server, int port,
				const std::string& request, int* byteRead, std::string* response, const std::string& proxy, const std::string& proxyBypass, DWORD iFlag = 0, int timeoutSeconds = 10);
			static SallyAPI::Network::NETWORK_RETURN GetHTTPContent(const std::string& server, int port,
				const std::string& request, int* byteRead, char** response, const std::string& proxy, const std::string& proxyBypass, DWORD iFlag = 0, int timeoutSeconds = 10);
			static SallyAPI::Network::NETWORK_RETURN GetFileContent(const std::string& server, int port,const std::string& request, int* byteRead, HANDLE hFile, const std::string& proxy, const std::string& proxyBypass, DWORD iFlag = 0, int timeoutSeconds = 10);
			static std::string	URLEncode(const std::string& stringToEncode);
			static bool			DownloadFile(const std::string& request, const std::string& imageFile, const std::string& proxy, const std::string& proxyBypass);
		};
	}
}
