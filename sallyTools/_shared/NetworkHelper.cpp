////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\NetworkHelper.cpp
///
/// \brief	Implements the network helper class. 
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

#include "NetworkHelper.h"

using namespace SallyAPI::Network;

std::string char2hex(char dec)
{
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
	if (10<= dig1 && dig1<=15) dig1+=97-10; //a,97inascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=97-10;

	std::string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}

SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetHTTPText(const std::string& server, int port, 
															 const std::string& request, int* byteRead,
															 std::string* response, const std::string& proxy,
															 const std::string& proxyBypass,
															 DWORD iFlag, int timeoutSeconds)
{
	HINTERNET connection = NULL;
	HINTERNET httpConnect = NULL;
	HINTERNET httpRequest = NULL;

	if (proxy.length() == 0)
		connection = InternetOpen("SallyAPI", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	else
		connection = InternetOpen("SallyAPI", INTERNET_OPEN_TYPE_PROXY, proxy.c_str(), proxyBypass.c_str(), 0);

	if (connection == NULL)
	{
		return ERROR_PREPARE;
	}
	httpConnect = InternetConnect(connection, server.c_str(), port, "", "", INTERNET_SERVICE_HTTP,0,0);
	if (httpConnect == NULL)
	{
		InternetCloseHandle(connection);
		return ERROR_PREPARE;
	}

	httpRequest = HttpOpenRequest(httpConnect, "GET", request.c_str(), NULL, NULL, NULL, iFlag | INTERNET_FLAG_RELOAD, 0);
	if (httpRequest == NULL)
	{
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);
		return ERROR_OPEN;
	}

	DWORD dwFlags;
	DWORD dwSize = sizeof(dwFlags);

	InternetQueryOption(httpRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwSize);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	InternetSetOption(httpRequest, INTERNET_OPTION_SECURITY_FLAGS,
		&dwFlags, sizeof(dwSize));

	DWORD	reqget = 20;
	char reqReceive[20];

	CHttpSendRequestThread httpSendRequestThread;
	httpSendRequestThread.SetValues(httpRequest);

	httpSendRequestThread.Start();

	int millSeconds = timeoutSeconds * 1000;
	do
	{
		Sleep(100);
		millSeconds -= 100;
	} while ((httpSendRequestThread.GetStatus() == SallyAPI::System::THREAD_RUNNING) && (millSeconds > 0));

	if (httpSendRequestThread.GetStatus() == SallyAPI::System::THREAD_RUNNING)
	{
		httpSendRequestThread.Stop(true);

		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);
		return ERROR_HTTP_TIMEOUT;
	}

	HttpQueryInfo(httpRequest,HTTP_QUERY_STATUS_TEXT ,reqReceive,&reqget,0);

	if (reqget == 0)
	{
		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		return ERROR_NOTHING_READ;
	}

	char		buf[1001];
	bool		readOk = true;
	DWORD		dBytesReceived;
	*byteRead = 0;

	do
	{
		readOk = (InternetReadFile(httpRequest, &buf, 1000, &dBytesReceived) != 0);
		if (dBytesReceived > 0)
		{
			buf[dBytesReceived] = '\0';
			response->append(buf);
			*byteRead += dBytesReceived;
		}				
	}
	while (readOk == TRUE && dBytesReceived > 0);

	InternetCloseHandle(httpRequest);
	InternetCloseHandle(httpConnect);
	InternetCloseHandle(connection);
	return SUCCESS;
}

SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetHTTPContent(const std::string& server, int port,
															   const std::string& request, int* byteRead,
															   char** response, const std::string& proxy,
															   const std::string& proxyBypass,
															   DWORD iFlag, int timeoutSeconds)
{
	HINTERNET connection = NULL;
	HINTERNET httpConnect = NULL;
	HINTERNET httpRequest = NULL;

	if (proxy.length() == 0)
		connection = InternetOpen("SallyAPI", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	else
		connection = InternetOpen("SallyAPI", INTERNET_OPEN_TYPE_PROXY, proxy.c_str(), proxyBypass.c_str(), 0);

	if (connection == NULL)
	{
		return ERROR_PREPARE;
	}
	httpConnect = InternetConnect(connection, server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP,0, 0);
	if (httpConnect == NULL)
	{
		InternetCloseHandle(connection);
		return ERROR_PREPARE;
	}

	httpRequest = HttpOpenRequest(httpConnect, "GET", request.c_str(), NULL, NULL, NULL,iFlag | INTERNET_FLAG_RELOAD, 0);
	if (httpRequest == NULL)
	{
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);
		return ERROR_OPEN;
	}

	DWORD dwFlags;
	DWORD dwSize = sizeof(dwFlags);

	InternetQueryOption(httpRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwSize);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	InternetSetOption(httpRequest, INTERNET_OPTION_SECURITY_FLAGS,
		&dwFlags, sizeof(dwSize));

	DWORD	reqget = 20;
	char reqReceive[20];

	CHttpSendRequestThread httpSendRequestThread;
	httpSendRequestThread.SetValues(httpRequest);

	httpSendRequestThread.Start();

	int millSeconds = timeoutSeconds * 1000;
	do
	{
		Sleep(100);
		millSeconds -= 100;
	} while ((httpSendRequestThread.GetStatus() == SallyAPI::System::THREAD_RUNNING) && (millSeconds > 0));

	if (httpSendRequestThread.GetStatus() == SallyAPI::System::THREAD_RUNNING)
	{
		httpSendRequestThread.Stop(true);

		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);
		return ERROR_HTTP_TIMEOUT;
	}

	HttpQueryInfo(httpRequest,HTTP_QUERY_STATUS_CODE ,reqReceive,&reqget,0);

	if (reqget == 0)
	{
		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		return ERROR_NOTHING_READ;
	}

	char		buf[1001];
	bool		readOk = true;
	DWORD		dBytesReceived;
	*byteRead = 0;

	do
	{
		readOk = (InternetReadFile(httpRequest, &buf, 1000, &dBytesReceived) != 0);
		DWORD error = GetLastError();
		if (dBytesReceived > 0)
		{
			*response = (char*) realloc(*response, (*byteRead + dBytesReceived) * sizeof(char));

			char* temp = *response + (*byteRead * sizeof(char));
			for (DWORD i = 0; i < dBytesReceived; ++i)
			{
				*temp = buf[i];
				++temp;
			}
			*byteRead += dBytesReceived;
		}				
	}
	while (readOk == TRUE && dBytesReceived > 0);

	InternetCloseHandle(httpRequest);
	InternetCloseHandle(httpConnect);
	InternetCloseHandle(connection);
	return SUCCESS;
}

SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetFileContent(const std::string& server, int port,
															   const std::string& request, int* byteRead,
															   HANDLE hFile, const std::string& proxy,
															   const std::string& proxyBypass,
															   DWORD iFlag, int timeoutSeconds)
{
	HINTERNET connection = NULL;
	HINTERNET httpConnect = NULL;
	HINTERNET httpRequest = NULL;

	if (proxy.length() == 0)
		connection = InternetOpen("SallyAPI", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	else
		connection = InternetOpen("SallyAPI", INTERNET_OPEN_TYPE_PROXY, proxy.c_str(), proxyBypass.c_str(), 0);

	if (connection == NULL)
	{
		return ERROR_PREPARE;
	}
	httpConnect = InternetConnect(connection, server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP,0, 0);
	if (httpConnect == NULL)
	{
		InternetCloseHandle(connection);
		return ERROR_PREPARE;
	}

	httpRequest = HttpOpenRequest(httpConnect, "GET", request.c_str(), NULL, NULL, NULL,iFlag | INTERNET_FLAG_RELOAD, 0);
	if (httpRequest == NULL)
	{
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);
		return ERROR_OPEN;
	}

	DWORD dwFlags;
	DWORD dwSize = sizeof(dwFlags);

	InternetQueryOption(httpRequest, INTERNET_OPTION_SECURITY_FLAGS, (LPVOID)&dwFlags, &dwSize);
	dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
	InternetSetOption(httpRequest, INTERNET_OPTION_SECURITY_FLAGS,
		&dwFlags, sizeof(dwSize));

	DWORD	reqget = 20;
	char reqReceive[20];

	CHttpSendRequestThread httpSendRequestThread;
	httpSendRequestThread.SetValues(httpRequest);

	httpSendRequestThread.Start();

	int millSeconds = timeoutSeconds * 1000;
	do
	{
		Sleep(100);
		millSeconds -= 100;
	} while ((httpSendRequestThread.GetStatus() == SallyAPI::System::THREAD_RUNNING) && (millSeconds > 0));

	if (httpSendRequestThread.GetStatus() == SallyAPI::System::THREAD_RUNNING)
	{
		httpSendRequestThread.Stop(true);

		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);
		return ERROR_HTTP_TIMEOUT;
	}

	HttpQueryInfo(httpRequest,HTTP_QUERY_STATUS_CODE ,reqReceive,&reqget,0);

	if (reqget == 0)
	{
		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		return ERROR_NOTHING_READ;
	}

	char		buf[1001];
	bool		readOk = true;
	DWORD		dBytesReceived;
	*byteRead = 0;

	do
	{
		readOk = (InternetReadFile(httpRequest, &buf, 1000, &dBytesReceived) != 0);
		DWORD error = GetLastError();
		if (dBytesReceived > 0)
		{
			DWORD		DWord;
			WriteFile(hFile, buf ,(DWORD) dBytesReceived, &DWord, 0);

			*byteRead += dBytesReceived;
		}				
	}
	while (readOk == TRUE && dBytesReceived > 0);

	InternetCloseHandle(httpRequest);
	InternetCloseHandle(httpConnect);
	InternetCloseHandle(connection);
	return SUCCESS;
}

std::string NetworkHelper::URLEncode(const std::string& stringToEncode)
{
	std::string escaped="";
	int max = stringToEncode.length();
	for(int i=0; i<max; i++)
	{
		if ( (48 <= stringToEncode[i] && stringToEncode[i] <= 57) ||//0-9
			(65 <= stringToEncode[i] && stringToEncode[i] <= 90) ||//abc...xyz
			(97 <= stringToEncode[i] && stringToEncode[i] <= 122) || //ABC...XYZ
			(stringToEncode[i]=='~' || stringToEncode[i]=='!' || stringToEncode[i]=='*' || stringToEncode[i]=='(' || stringToEncode[i]==')' || stringToEncode[i]=='\'')
			)
		{
			escaped.append( &stringToEncode[i], 1);
		}
		else
		{
			escaped.append("%");
			escaped.append( char2hex(stringToEncode[i]) );//converts char 255 to string "ff"
		}
	}
	return escaped;
}

bool NetworkHelper::DownloadFile(const std::string& request, const std::string& imageFile, const std::string& proxy, const std::string& proxyBypass)
{
	int byteRead = 0;

	HANDLE hFile = CreateFile(imageFile.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if ((hFile == INVALID_HANDLE_VALUE) && (hFile == NULL))
		return false;

	SallyAPI::Network::NETWORK_RETURN networkReturn = SallyAPI::Network::NetworkHelper::GetFileContent(COMMUNITY_SERVER, COMMUNITY_PORT, request, &byteRead, hFile, proxy, proxyBypass);

	CloseHandle(hFile);

	if (networkReturn != SallyAPI::Network::SUCCESS)
		return false;
	return true;
}