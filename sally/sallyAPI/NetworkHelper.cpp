////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\NetworkHelper.cpp
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

#include "NetworkHelper.h"
#include "Game.h"

using namespace SallyAPI::Network;

#define array_length(array) (sizeof (array) / sizeof (array)[0])

std::string char2hex(char dec)
{
	char dig1 = (dec&0xF0)>>4;
	char dig2 = (dec&0x0F);
	if ( 0<= dig1 && dig1<= 9) dig1+=48;    //0,48inascii
	if (10<= dig1 && dig1<=15) dig1+=65-10; //a,65inascii
	if ( 0<= dig2 && dig2<= 9) dig2+=48;
	if (10<= dig2 && dig2<=15) dig2+=97-10;

	std::string r;
	r.append( &dig1, 1);
	r.append( &dig2, 1);
	return r;
}

struct HTMLReplace {
	std::string match;
	std::string replace;
} codes[] = {
	{"&", "&amp;"},
	{"<", "&lt;"}, 
	{">", "&gt;"},
	{"ü", "&uuml;"}
};

void LogNetworkError(const std::string& server, int port, const std::string& request, SallyAPI::Network::NETWORK_RETURN error)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	std::string errorString = "Network error:\n";
	errorString.append("                    Server:  ");
	errorString.append(server);
	errorString.append("\n");
	errorString.append("                    Port:    ");
	errorString.append(SallyAPI::String::StringHelper::ConvertToString(port));
	errorString.append("\n");
	errorString.append("                    Request: ");
	errorString.append(request);
	errorString.append("\n");
	errorString.append("                    Error:   ");
	errorString.append(SallyAPI::String::StringHelper::ConvertToString((int) error));

	logger->Debug(errorString);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetHTTPText(const std::string& server,
/// int port, const std::string& request, int* byteRead, std::string* response,
/// const std::string& proxy, const std::string& proxyBypass, DWORD iFlag, int timeoutSeconds,
/// std::string* headerData, std::string* postData)
///
/// \brief	Gets a http text. 
///
/// \author	Christian Knobloch
/// \date	17.12.2010
///
/// \param	server				The server. 
/// \param	port				The port. 
/// \param	request				The request. 
/// \param [in,out]	byteRead	If non-null, the byte read. 
/// \param [in,out]	response	If non-null, the response. 
/// \param	proxy				The proxy. 
/// \param	proxyBypass			The proxy bypass. 
/// \param	iFlag				The flag. 
/// \param	timeoutSeconds		The timeout in seconds. 
/// \param [in,out]	headerData	If non-null, information describing the header. 
/// \param [in,out]	postData	If non-null, information describing the post. 
///
/// \return	The http text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetHTTPText(const std::string& server, int port, 
															 const std::string& request, int* byteRead,
															 std::string* response, const std::string& proxy,
															 const std::string& proxyBypass,
															 DWORD iFlag, int timeoutSeconds,
															 std::string* headerData, std::string* postData)
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
		LogNetworkError(server, port, request, ERROR_PREPARE);
		return ERROR_PREPARE;
	}
	httpConnect = InternetConnect(connection, server.c_str(), port, "", "", INTERNET_SERVICE_HTTP,0,0);
	if (httpConnect == NULL)
	{
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_PREPARE);
		return ERROR_PREPARE;
	}

	std::string sendMethod = "GET";
	if (postData != NULL)
		sendMethod = "POST";

	httpRequest = HttpOpenRequest(httpConnect, sendMethod.c_str(), request.c_str(), NULL, NULL, NULL, iFlag | INTERNET_FLAG_RELOAD, 0);
	if (httpRequest == NULL)
	{
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_OPEN);
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
	if (postData != NULL)
		httpSendRequestThread.SetValues(httpRequest, *postData);
	else
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

		LogNetworkError(server, port, request, ERROR_HTTP_TIMEOUT);
		return ERROR_HTTP_TIMEOUT;
	}

	HttpQueryInfo(httpRequest,HTTP_QUERY_STATUS_TEXT ,reqReceive,&reqget,0);

	if (reqget == 0)
	{
		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_NOTHING_READ);
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

	if (headerData != NULL)
		GetHeaderData(httpRequest, headerData);

	InternetCloseHandle(httpRequest);
	InternetCloseHandle(httpConnect);
	InternetCloseHandle(connection);
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetHTTPContent(const std::string& server,
/// int port, const std::string& request, int* byteRead, char** response,
/// const std::string& proxy, const std::string& proxyBypass, DWORD iFlag, int timeoutSeconds,
/// std::string* headerData, std::string* postData)
///
/// \brief	Gets a http content. 
///
/// \author	Christian Knobloch
/// \date	17.12.2010
///
/// \param	server				The server. 
/// \param	port				The port. 
/// \param	request				The request. 
/// \param [in,out]	byteRead	If non-null, the byte read. 
/// \param [in,out]	response	If non-null, the response. 
/// \param	proxy				The proxy. 
/// \param	proxyBypass			The proxy bypass. 
/// \param	iFlag				The flag. 
/// \param	timeoutSeconds		The timeout in seconds. 
/// \param [in,out]	headerData	If non-null, information describing the header. 
/// \param [in,out]	postData	If non-null, information describing the post. 
///
/// \return	The http content. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetHTTPContent(const std::string& server, int port,
															   const std::string& request, int* byteRead,
															   char** response, const std::string& proxy,
															   const std::string& proxyBypass,
															   DWORD iFlag, int timeoutSeconds,
															   std::string* headerData, std::string* postData)
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
		LogNetworkError(server, port, request, ERROR_PREPARE);
		return ERROR_PREPARE;
	}
	httpConnect = InternetConnect(connection, server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP,0, 0);
	if (httpConnect == NULL)
	{
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_PREPARE);
		return ERROR_PREPARE;
	}

	std::string sendMethod = "GET";
	if (postData != NULL)
		sendMethod = "POST";

	httpRequest = HttpOpenRequest(httpConnect, sendMethod.c_str(), request.c_str(), NULL, NULL, NULL,iFlag | INTERNET_FLAG_RELOAD, 0);
	if (httpRequest == NULL)
	{
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_OPEN);
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
	if (postData != NULL)
		httpSendRequestThread.SetValues(httpRequest, *postData);
	else
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

		LogNetworkError(server, port, request, ERROR_HTTP_TIMEOUT);
		return ERROR_HTTP_TIMEOUT;
	}

	HttpQueryInfo(httpRequest,HTTP_QUERY_STATUS_CODE ,reqReceive,&reqget,0);

	if (reqget == 0)
	{
		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_NOTHING_READ);
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

	if (headerData != NULL)
		GetHeaderData(httpRequest, headerData);

	InternetCloseHandle(httpRequest);
	InternetCloseHandle(httpConnect);
	InternetCloseHandle(connection);
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void NetworkHelper::GetHeaderData(HINTERNET httpRequest, std::string* headerData)
///
/// \brief	Gets a header data from an HINTERNET request
///
/// \author	Christian Knobloch
/// \date	17.12.2010
///
/// \param	httpRequest			The http request. 
/// \param [in,out]	headerData	If non-null, information describing the header. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void NetworkHelper::GetHeaderData(HINTERNET httpRequest, std::string* headerData)
{
	DWORD dwSize = 0;

	// This call will fail on the first pass, because 
	// no buffer is allocated.
	HttpQueryInfo(httpRequest, HTTP_QUERY_RAW_HEADERS_CRLF, NULL, &dwSize, NULL);
	if (GetLastError() == ERROR_HTTP_HEADER_NOT_FOUND)
		return;

	// Check for an insufficient buffer.
	// Allocate the necessary buffer.
	char* lpOutBuffer = new char[dwSize];

	HttpQueryInfo(httpRequest, HTTP_QUERY_RAW_HEADERS_CRLF, (LPVOID)lpOutBuffer, &dwSize, NULL);

	headerData->append(lpOutBuffer);

	delete [] lpOutBuffer;
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetFileContent(const std::string& server,
/// int port, const std::string& request, int* byteRead, HANDLE hFile, const std::string& proxy,
/// const std::string& proxyBypass, DWORD iFlag, int timeoutSeconds, std::string* headerData,
/// std::string* postData)
///
/// \brief	Gets a file content. 
///
/// \author	Christian Knobloch
/// \date	17.12.2010
///
/// \param	server				The server. 
/// \param	port				The port. 
/// \param	request				The request. 
/// \param [in,out]	byteRead	If non-null, the byte read. 
/// \param	hFile				Handle of the file. 
/// \param	proxy				The proxy. 
/// \param	proxyBypass			The proxy bypass. 
/// \param	iFlag				The flag. 
/// \param	timeoutSeconds		The timeout in seconds. 
/// \param [in,out]	headerData	If non-null, information describing the header. 
/// \param [in,out]	postData	If non-null, information describing the post. 
///
/// \return	The file content. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Network::NETWORK_RETURN NetworkHelper::GetFileContent(const std::string& server, int port,
															   const std::string& request, int* byteRead,
															   HANDLE hFile, const std::string& proxy,
															   const std::string& proxyBypass,
															   DWORD iFlag, int timeoutSeconds,
															   std::string* headerData, std::string* postData)
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
		LogNetworkError(server, port, request, ERROR_PREPARE);
		return ERROR_PREPARE;
	}
	httpConnect = InternetConnect(connection, server.c_str(), port, NULL, NULL, INTERNET_SERVICE_HTTP,0, 0);
	if (httpConnect == NULL)
	{
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_PREPARE);
		return ERROR_PREPARE;
	}

	std::string sendMethod = "GET";
	if (postData != NULL)
		sendMethod = "POST";

	httpRequest = HttpOpenRequest(httpConnect, sendMethod.c_str(), request.c_str(), NULL, NULL, NULL,iFlag | INTERNET_FLAG_RELOAD, 0);
	if (httpRequest == NULL)
	{
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_OPEN);
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
	if (postData != NULL)
		httpSendRequestThread.SetValues(httpRequest, *postData);
	else
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

		LogNetworkError(server, port, request, ERROR_HTTP_TIMEOUT);
		return ERROR_HTTP_TIMEOUT;
	}

	HttpQueryInfo(httpRequest,HTTP_QUERY_STATUS_CODE ,reqReceive,&reqget,0);

	if (reqget == 0)
	{
		InternetCloseHandle(httpRequest);
		InternetCloseHandle(httpConnect);
		InternetCloseHandle(connection);

		LogNetworkError(server, port, request, ERROR_NOTHING_READ);
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

	if (headerData != NULL)
		GetHeaderData(httpRequest, headerData);

	InternetCloseHandle(httpRequest);
	InternetCloseHandle(httpConnect);
	InternetCloseHandle(connection);
	return SUCCESS;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string NetworkHelper::URLEncode(const std::string& stringToEncode)
///
/// \brief	Url encode. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	stringToEncode	The string to encode. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string HTMLEncode(const std::string& stringToEncode)
///
/// \brief	Html encode. 
///
/// \author	Christian Knobloch
/// \date	08.07.2010
///
/// \param	stringToEncode	The string to encode. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string NetworkHelper::HTMLEncode(const std::string& stringToEncode)
{
	std::string rs = stringToEncode;

	// Replace each matching token in turn
	for (std::size_t i = 0; i < array_length(codes); i++)
	{
		// Find the first match
		const std::string& match = codes[i].match;
		const std::string& repl = codes[i].replace;
		std::string::size_type start = rs.find_first_of(match);

		// Replace all matches
		while (start != std::string::npos)
		{
			rs.replace(start, match.size(), repl);
			// Be sure to jump forward by the replacement length
			start = rs.find_first_of(match, start + repl.size());
		}
	}

	return rs;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool NetworkHelper::DownloadFile(const std::string& server, int port,
/// const std::string& request, const std::string& imageFile, const std::string& proxy,
/// const std::string& proxyBypass)
///
/// \brief	Downloads a file. 
///
/// \author	Christian Knobloch
/// \date	13.05.2010
///
/// \param	server		The server. 
/// \param	port		The port. 
/// \param	request		The request. 
/// \param	imageFile	The image file. 
/// \param	proxy		The proxy. 
/// \param	proxyBypass	The proxy bypass. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool NetworkHelper::DownloadFile(const std::string& server, int port,
								 const std::string& request, const std::string& imageFile, 
								 const std::string& proxy, const std::string& proxyBypass)
{
	int byteRead = 0;

	HANDLE hFile = CreateFile(imageFile.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
	if ((hFile == INVALID_HANDLE_VALUE) && (hFile == NULL))
		return false;

	SallyAPI::Network::NETWORK_RETURN networkReturn = SallyAPI::Network::NetworkHelper::GetFileContent(server, port, request, &byteRead, hFile, proxy, proxyBypass);

	CloseHandle(hFile);

	if (networkReturn != SallyAPI::Network::SUCCESS)
		return false;
	return true;
}