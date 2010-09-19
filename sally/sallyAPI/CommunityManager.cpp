////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\CommunityManager.cpp
///
/// \brief	Implements the community manager class. 
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

#include "CommunityManager.h"

SallyAPI::Community::CCommunityManager*	SallyAPI::Community::CCommunityManager::m_pObject = NULL;

using namespace SallyAPI::Community;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCommunityManager::CCommunityManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCommunityManager::CCommunityManager()
{
	m_bLastRequestSuccess = true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CCommunityManager::~CCommunityManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CCommunityManager::~CCommunityManager()
{
	m_tCommunityThread.WaitForStop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCommunityManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommunityManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Community::CCommunityManager* CCommunityManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Community::CCommunityManager* CCommunityManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Community::CCommunityManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCommunityManager::IsEnabled()
///
/// \brief	Query if this object is enabled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if enabled, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCommunityManager::IsEnabled()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	return option->GetPropertyBool("sally", "community", false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CCommunityManager::GetUsername()
///
/// \brief	Gets the username. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The username. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CCommunityManager::GetUsername()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	return option->GetPropertyString("sally", "communityUsername", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CCommunityManager::GetPassword()
///
/// \brief	Gets the password. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The password. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CCommunityManager::GetPassword()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	return option->GetPropertyString("sally", "communityPassword", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CCommunityManager::GenerateBaseRequest(const std::string& menu)
///
/// \brief	Generates a base request. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	menu	The menu. 
///
/// \return	The base request. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CCommunityManager::GenerateBaseRequest(const std::string& menu)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string userName = option->GetPropertyString("sally", "communityUsername", "");
	std::string password = option->GetPropertyString("sally", "communityPassword", "");

	std::string request = COMMUNITY_URL;
	request.append("rest.php?menu=");
	request.append(menu);
	request.append("&Username=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(userName));
	request.append("&Password=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(password));

	return request;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCommunityManager::TestLogin(std::string& errorMessage)
///
/// \brief	Tests login. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	errorMessage	Message describing the error. 
///
/// \return	true if the test passes, false if the test fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCommunityManager::TestLogin(std::string& errorMessage)
{
	if (!IsEnabled())
	{
		errorMessage = "Sally community is not enabled.";
		return "";
	}

	std::string request = GenerateBaseRequest("login");

	std::string response;
	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NETWORK_RETURN networkReturn = SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT, request, &byteRead, &response, proxy, proxyBypass);

	switch (networkReturn)
	{
	case SallyAPI::Network::ERROR_PREPARE:
		m_strErrorMessage = errorMessage = "Network preparation failed";
		m_strErrorMessage = m_bLastRequestSuccess = false;
		return false;
	case SallyAPI::Network::ERROR_OPEN:
		m_strErrorMessage = errorMessage = "Network open failed";
		m_bLastRequestSuccess = false;
		return false;
	case SallyAPI::Network::ERROR_HTTP_TIMEOUT:
		m_strErrorMessage = errorMessage = "HTTP Timeout";
		m_bLastRequestSuccess = false;
		return false;
	case SallyAPI::Network::ERROR_NOTHING_READ:
		m_strErrorMessage = errorMessage = "Nothing read";
		m_bLastRequestSuccess = false;
		return false;
	default:
		break;
	}

	if (response.find("<sallycommunity login=\"true\"") == std::string::npos)
	{
		m_strErrorMessage = errorMessage = "Login error\n(wrong username or password)";
		m_bLastRequestSuccess = false;
		return false;
	}

	m_strErrorMessage = "";
	m_bLastRequestSuccess = true;
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CCommunityManager::RequestData(const std::string& action,
/// std::map<std::string, std::string>& requestMap, std::string& errorMessage)
///
/// \brief	Request data. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	action					The action. 
/// \param [in,out]	requestMap		The request map. 
/// \param [in,out]	errorMessage	Message describing the error. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CCommunityManager::RequestData(const std::string& action, std::map<std::string, std::string>& requestMap,
										   std::string& errorMessage)
{
	if (!IsEnabled())
	{
		errorMessage = "Sally community is not enabled.";
		return "";
	}

	std::string request = GenerateBaseRequest(action);

	// add request map to request
	std::map<std::string, std::string>::iterator iter = requestMap.begin();

	while(iter != requestMap.end())
	{
		std::string key = iter->first;
		std::string value = iter->second;

		request.append("&");
		request.append(key);
		request.append("=");
		request.append(SallyAPI::Network::NetworkHelper::URLEncode(value));

		++iter;
	}

	// make request
	std::string response;
	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NETWORK_RETURN networkReturn = SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT, request, &byteRead, &response, proxy, proxyBypass);

	switch (networkReturn)
	{
	case SallyAPI::Network::ERROR_PREPARE:
		m_strErrorMessage = errorMessage = "Network preparation failed";
		m_bLastRequestSuccess = false;
		return "";
	case SallyAPI::Network::ERROR_OPEN:
		m_strErrorMessage = errorMessage = "Network open failed";
		m_bLastRequestSuccess = false;
		return "";
	case SallyAPI::Network::ERROR_HTTP_TIMEOUT:
		m_strErrorMessage = errorMessage = "HTTP Timeout";
		m_bLastRequestSuccess = false;
		return "";
	case SallyAPI::Network::ERROR_NOTHING_READ:
		m_strErrorMessage = errorMessage = "Nothing read";
		m_bLastRequestSuccess = false;
		return "";
	default:
		break;
	}

	if (response.find("<sallycommunity login=\"true\"") == std::string::npos)
	{
		m_strErrorMessage = errorMessage = "Login error\n(wrong username or password)";
		m_bLastRequestSuccess = false;
		return "";
	}

	m_strErrorMessage = "";
	m_bLastRequestSuccess = true;
	return response;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCommunityManager::SendData(const std::string& action, std::map<std::string,
/// std::string>& requestMap)
///
/// \brief	Send data. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	action				The action. 
/// \param [in,out]	requestMap	The request map. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCommunityManager::SendData(const std::string& action, std::map<std::string, std::string>& requestMap)
{
	if (!IsEnabled())
		return false;

	std::string request = GenerateBaseRequest(action);

	// add request map to request
	std::map<std::string, std::string>::iterator iter = requestMap.begin();

	while(iter != requestMap.end())
	{
		std::string key = iter->first;
		std::string value = iter->second;

		request.append("&");
		request.append(key);
		request.append("=");
		request.append(SallyAPI::Network::NetworkHelper::URLEncode(value));

		++iter;
	}
	m_tCommunityThread.Request(request);
	m_tCommunityThread.Start();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCommunityManager::SendStatusMessage(const std::string& explicidAppName,
/// const std::string& appName, const std::string& message, const std::string& action,
/// const std::string& actionName)
///
/// \brief	Send status message. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	appName			Name of the application. 
/// \param	message			The message. 
/// \param	action			The action. 
/// \param	actionName		Name of the action. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCommunityManager::SendStatusMessage(const std::string& explicidAppName, const std::string& appName,
										  const std::string& message, const std::string& action,
										  const std::string& actionName)
{
	if (!IsEnabled())
		return false;

	std::map<std::string, std::string> requestMap;

	requestMap["explicidAppName"] = explicidAppName;
	requestMap["appName"] = appName;
	requestMap["message"] = message;
	requestMap["action"] = action;
	requestMap["actionName"] = actionName;

	return SendData("sendStatusMessage", requestMap);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCommunityManager::SendUpdateToRegistedNotifier()
///
/// \brief	Send update to registed notifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommunityManager::SendUpdateToRegistedNotifier()
{
	// send to registered notifier
	std::vector<SallyAPI::GUI::CGUIBaseObject*>::iterator iter = m_pNotifierWindows.begin();
	while (iter != m_pNotifierWindows.end())
	{
		SallyAPI::GUI::CGUIBaseObject* temp = *iter;

		temp->SendMessageToParent(temp, 0, MS_SALLY_COMMUNITY_STATUS_UPDATE);
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CCommunityManager::RegisterStatusUpdateNotifier(SallyAPI::GUI::CGUIBaseObject* window)
///
/// \brief	Register status update notifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	window	If non-null, the window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CCommunityManager::RegisterStatusUpdateNotifier(SallyAPI::GUI::CGUIBaseObject* window)
{
	m_pNotifierWindows.push_back(window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CCommunityManager::LastRequestSuccess()
///
/// \brief	Was the last request successful. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CCommunityManager::LastRequestSuccess()
{
	return m_bLastRequestSuccess;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CCommunityManager::GetLastRequestErrorMessage()
///
/// \brief	Gets the last request error message. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The last request error message. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CCommunityManager::GetLastRequestErrorMessage()
{
	return m_strErrorMessage;
}