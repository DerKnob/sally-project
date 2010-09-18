////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyUpdate\NetworkHelper.h
///
/// \brief	Declares the network helper functions. 
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

std::string GetProxy()
{
	if (SallyAPI::System::COption::GetPropertyBoolStatic("network", "useInternetExplorer", true, iniFile) == false)
	{
		return SallyAPI::System::COption::GetPropertyStringStatic("network", "proxyServer", "", iniFile);
	}
	else
	{
		unsigned long nSize = 4096;
		char szBuf[4096] =
		{
			0
		};
		INTERNET_PROXY_INFO* pInfo = (INTERNET_PROXY_INFO*)szBuf;
		if (!InternetQueryOption(NULL, INTERNET_OPTION_PROXY, pInfo, &nSize))
		{
			return "";
		}

		if (pInfo->dwAccessType != INTERNET_OPEN_TYPE_DIRECT)
		{
			std::string proxy = pInfo->lpszProxy;
			return proxy;
		}
	}
	// No proxy set
	return "";
}

std::string GetProxyBypass()
{
	if (SallyAPI::System::COption::GetPropertyBoolStatic("network", "useInternetExplorer", true, iniFile) == false)
	{
		return SallyAPI::System::COption::GetPropertyStringStatic("network", "proxyBypass", "", iniFile);
	}
	else
	{
		unsigned long nSize = 4096;
		char szBuf[4096] =
		{
			0
		};
		INTERNET_PROXY_INFO* pInfo = (INTERNET_PROXY_INFO*)szBuf;
		if (!InternetQueryOption(NULL, INTERNET_OPTION_PROXY, pInfo, &nSize))
		{
			return "";
		}

		if (pInfo->dwAccessType != INTERNET_OPEN_TYPE_DIRECT)
		{
			std::string proxyBypass = pInfo->lpszProxyBypass;
			return proxyBypass;
		}
		// No proxyBypass set
	}
	return "";
}

void DownloadFile(const std::string& link, const std::string& versionName)
{
	std::string fileName = searchUpdatesTemp;
	fileName.append(versionName);

	std::string requestURI = COMMUNITY_URL;
	requestURI.append(link);

	std::string proxy = GetProxy();
	std::string proxyBypass = GetProxyBypass();
	if (!SallyAPI::Network::NetworkHelper::DownloadFile(requestURI, fileName, proxy, proxyBypass))
		return;

	std::string fileNameNew = searchUpdates;
	fileNameNew.append(versionName);

	// move from temp to install folder
	MoveFile(fileName.c_str(), fileNameNew.c_str());
}

std::string RequestFile(const std::string& uid, const std::string& currentVersion, const std::string& currentAPIVersion)
{
	// make the connection
	std::string request = COMMUNITY_URL;
	request.append("checkUpdate.php");

	request.append("?type=");

	if (SallyAPI::System::COption::GetPropertyBoolStatic("sally", "beta", false, iniFile))
	{
		request.append("beta");
	}	

	request.append("&uid=");
	request.append(uid);

	request.append("&apiVersion=");
	request.append(currentAPIVersion);

	std::string response;
	int byteRead = 0;

	std::string proxy = GetProxy();
	std::string proxyBypass = GetProxyBypass();
	SallyAPI::Network::NETWORK_RETURN networkReturn = SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT, request, &byteRead, &response, proxy, proxyBypass);


	switch (networkReturn)
	{
	case SallyAPI::Network::ERROR_PREPARE:
		return "";
	case SallyAPI::Network::ERROR_OPEN:
		return "";
	case SallyAPI::Network::ERROR_HTTP_TIMEOUT:
		return "";
	case SallyAPI::Network::ERROR_NOTHING_READ:
		return "";
	default:
		break;
	}

	std::string tempFile = searchUpdatesTemp;
	tempFile.append("updateInfo.xml");

	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, response);

	XMLNode xMainNode = XMLNode::openFileHelper(tempFile.c_str());
	if (xMainNode.isEmpty())
		return "";

	XMLNode sallycommunity = xMainNode.getChildNode("sallycommunity");
	if (sallycommunity.isEmpty())
		return "";

	std::string onlineVersion = CheckForNull(sallycommunity.getAttribute("lastVersion"));
	std::string apiVersion = CheckForNull(sallycommunity.getAttribute("apiVersion"));
	std::string link = CheckForNull(sallycommunity.getAttribute("link"));
	std::string filename = CheckForNull(sallycommunity.getAttribute("filename"));

	if (newVersion(currentVersion, onlineVersion))
	{
		DownloadFile(link, filename);
	}

	// cleanup
	DeleteFile(tempFile.c_str());

	// get the current
	if (newVersion(currentVersion, onlineVersion))
	{
		if (apiVersion.length() == 0)
		{
			apiVersion = currentAPIVersion;
		}
	}
	return apiVersion;
}