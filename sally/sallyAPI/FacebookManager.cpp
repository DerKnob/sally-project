////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FacebookManager.cpp
///
/// \brief	Implements the facebook manager class. 
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

#include "FacebookManager.h"
#include "FacebookDB.h"
#include "ParameterInfoPopup.h"

#pragma comment(lib, "xmlparser.lib")
#include <xmlParser.h>

SallyAPI::Facebook::CFacebookManager*	SallyAPI::Facebook::CFacebookManager::m_pObject = NULL;

using namespace SallyAPI::Facebook;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFacebookManager::CFacebookManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFacebookManager::CFacebookManager()
{
	m_iUserImageCounter = FACEBOOK_USER_IMAGES;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFacebookManager::~CFacebookManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFacebookManager::~CFacebookManager()
{
	m_tFacebookThread.WaitForStop();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Facebook::CFacebookManager* CFacebookManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Facebook::CFacebookManager* CFacebookManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Facebook::CFacebookManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookManager::IsEnabled()
///
/// \brief	Query if this object is enabled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if enabled, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFacebookManager::IsEnabled()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	return option->GetPropertyBool("sally", "facebook", false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFacebookManager::GenerateBaseRequest(const std::string& menu)
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

std::string CFacebookManager::GenerateBaseRequest(const std::string& menu)
{

	std::string request = COMMUNITY_URL;
	request.append("rest_2.0.php?menu=");
	request.append(menu);
	request.append("&UserID=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(GetFacebookUserId()));
	request.append("&SallyKey=");
	request.append(SallyAPI::Network::NetworkHelper::URLEncode(GetSallyKey()));

	return request;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFacebookManager::RequestData(const std::string& action, std::map<std::string,
/// std::string>& requestMap, std::string& errorMessage,
/// SallyAPI::Network::NETWORK_RETURN& errorCode)
///
/// \brief	Request data. 
///
/// \author	Christian Knobloch
/// \date	28.01.2011
///
/// \param	action					The action. 
/// \param [in,out]	requestMap		The request map. 
/// \param [in,out]	errorMessage	Message describing the error. 
/// \param [in,out]	errorCode		The error code. 
///
/// \return	. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFacebookManager::RequestData(const std::string& action, std::map<std::string, std::string>& requestMap,
										  std::string& errorMessage, SallyAPI::Network::NETWORK_RETURN& errorCode)
{
	if (!IsEnabled())
	{
		errorMessage = "Facebook is not enabled.";
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
	errorCode = SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT, request, &byteRead, &response, proxy, proxyBypass);

	switch (errorCode)
	{
	case SallyAPI::Network::ERROR_PREPARE:
		errorMessage = "Network preparation failed";
		return "";
	case SallyAPI::Network::ERROR_OPEN:
		errorMessage = "Network open failed";
		return "";
	case SallyAPI::Network::ERROR_HTTP_TIMEOUT:
		errorMessage = "HTTP Timeout";
		return "";
	case SallyAPI::Network::ERROR_NOTHING_READ:
		errorMessage = "Nothing read";
		return "";
	default:
		break;
	}

	if (response.find("<error>") != std::string::npos)
	{
		errorMessage = "Login error";
		return "";
	}

	return response;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookManager::SendData(const std::string& action, std::map<std::string,
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

bool CFacebookManager::SendData(const std::string& action, std::map<std::string, std::string>& requestMap)
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
	m_tFacebookThread.Request(COMMUNITY_SERVER, COMMUNITY_PORT, request);
	m_tFacebookThread.Start();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookManager::SendStatusMessage(const std::string& explicidAppName,
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

bool CFacebookManager::SendStatusMessage(const std::string& explicidAppName, const std::string& appName,
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

	return SendData("fbSendStatusMessage", requestMap);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::ShowErrorMessage(SallyAPI::GUI::CGUIBaseObject* mainWindow)
///
/// \brief	Shows the a popup error message. Is called when the facebook connect is not working
///         anymore.
///
/// \author	Christian Knobloch
/// \date	24.12.2010
///
/// \param [in,out]	mainWindow	If non-null, the main window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::ShowErrorMessage(SallyAPI::GUI::CGUIBaseObject* mainWindow)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* languageManager = config->GetLanguageLocalization();

	std::string infoHeader = languageManager->GetString("Facebook Connection Error");
	std::string infoMessage = languageManager->GetString("Maybe the server is currently not reachable or the internet connection is down.\nIf the error persists, please go to the config and activate the 'Facebook Connection' again.");

	// send the popupinfo
	SallyAPI::GUI::SendMessage::CParameterInfoPopup sendMessageParameterInfoPopup(GUI_THEME_SALLY_FACEBOOK,
		infoHeader, infoMessage);
	mainWindow->SendMessageToParent(mainWindow, 0, MS_SALLY_SHOW_INFO_POPUP, &sendMessageParameterInfoPopup);

	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
	logger->Warning("Facebook Connection Error");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookManager::UpdateFacebookUserInfo()
///
/// \brief	Updates a facebook user information. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFacebookManager::UpdateFacebookUserInfo()
{
	if (!IsEnabled())
		return false;

	std::string request = GenerateBaseRequest("fbUpdateUserInfo");	
	std::string post = "&access_token=";
	post.append(SallyAPI::Network::NetworkHelper::URLEncode(GetFacebookAccessToken()));

	std::string response;
	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NETWORK_RETURN networkReturn =
		SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT,
		request, &byteRead, &response, proxy, proxyBypass, 0, 10, NULL, &post);

	switch (networkReturn)
	{
	case SallyAPI::Network::ERROR_PREPARE:
		return false;
	case SallyAPI::Network::ERROR_OPEN:
		return false;
	case SallyAPI::Network::ERROR_HTTP_TIMEOUT:
		return false;
	case SallyAPI::Network::ERROR_NOTHING_READ:
		return true; // no network connection
	default:
		break;
	}

	if (response.find("<error>") != std::string::npos)
		return false;

	std::string tempFile = SallyAPI::Core::CGame::GetMediaFolder();
	tempFile.append("facebookUpdateUserInfo.xml");

	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, response);

	XMLNode xMainNode = XMLNode::parseFile(tempFile.c_str());
	if (xMainNode.isEmpty())
		return false;

	XMLNode root = xMainNode.getChildNode("sallycommunity");
	if (root.isEmpty())
		return false;

	// create
	XMLNode meXML;
	int i = 0;
	do
	{
		meXML = root.getChildNode("me", i);

		if (!meXML.isEmpty())
		{
			std::string name = CheckForNull(meXML.getAttribute("name"));

			SetFacebookUserName(name);
		}
		++i;
	}
	while (!meXML.isEmpty());

	// cleanup
	DeleteFile(tempFile.c_str());

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	*/ bool CFacebookManager::ReloadAllFacebookUserImages()
///
/// \brief	Reload all facebook user images. 
///
/// \author	Christian Knobloch
/// \date	27.12.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFacebookManager::ReloadAllFacebookUserImages()
{
	SallyAPI::Facebook::CFacebookDB* facebookDB = SallyAPI::Facebook::CFacebookDB::GetInstance();

	std::string imageFolder = SallyAPI::Core::CGame::GetMediaFolder();;
	imageFolder.append("Facebook\\");

	CreateDirectory(imageFolder.c_str(), NULL);

	LoadFacebookUserImage(GetFacebookUserId());

	std::vector<std::string> friends = facebookDB->GetFriendIds();
	std::vector<std::string>::iterator iter = friends.begin();

	while (iter != friends.end())
	{
		std::string f = *iter;

		LoadFacebookUserImage(f);

		++iter;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::DownloadFacebookUserImage(const std::string& imageFolder,
/// const std::string& userId)
///
/// \brief	Downloads a facebook user image. 
///
/// \author	Christian Knobloch
/// \date	27.12.2010
///
/// \param	imageFolder	Pathname of the image folder. 
/// \param	userId		Identifier for the user. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::DownloadFacebookUserImage(const std::string& imageFolder, const std::string& userId)
{
	std::string imageFile = imageFolder;
	imageFile.append(userId);
	imageFile.append(".jpg");

	std::string imageFileTemp = imageFile;
	imageFileTemp.append(".tmp");

	DeleteFile(imageFileTemp.c_str());

	std::string requestURI = "/";
	requestURI.append(userId);
	requestURI.append("/picture");

	// could the file be downloaded?
	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NetworkHelper::DownloadFile("graph.facebook.com", 80, requestURI, imageFileTemp, proxy, proxyBypass);

	if (MoveFileEx(imageFileTemp.c_str(), imageFile.c_str(), MOVEFILE_REPLACE_EXISTING) != 0)
		LoadFacebookUserImage(userId);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::SendUpdateToRegistedNotifier()
///
/// \brief	Send update to registed notifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::SendUpdateToRegistedNotifier()
{
	// send to registered notifier
	std::vector<SallyAPI::GUI::CGUIBaseObject*>::iterator iter = m_pNotifierWindows.begin();
	while (iter != m_pNotifierWindows.end())
	{
		SallyAPI::GUI::CGUIBaseObject* temp = *iter;

		temp->SendMessageToParent(temp, 0, MS_SALLY_FACEBOOK_STATUS_UPDATE);
		++iter;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::RegisterStatusUpdateNotifier(SallyAPI::GUI::CGUIBaseObject* window)
///
/// \brief	Register status update notifier. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	window	If non-null, the window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::RegisterStatusUpdateNotifier(SallyAPI::GUI::CGUIBaseObject* window)
{
	m_pNotifierWindows.push_back(window);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFacebookManager::GetFacebookAccessToken()
///
/// \brief	Gets the facebook access token. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \return	The facebook access token. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFacebookManager::GetFacebookAccessToken()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	return option->GetPropertyString("sally", "facebookAccessToken", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFacebookManager::GetFacebookUserName()
///
/// \brief	Gets the facebook user name. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \return	The facebook user name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFacebookManager::GetFacebookUserName()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	return option->GetPropertyString("sally", "facebookUserName", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFacebookManager::GetFacebookUserId()
///
/// \brief	Gets the facebook user identifier. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \return	The facebook user identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFacebookManager::GetFacebookUserId()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	std::string test = option->GetPropertyString("sally", "facebookUserId", "");

	return option->GetPropertyString("sally", "facebookUserId", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CFacebookManager::GetSallyKey()
///
/// \brief	Gets the sally key. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \return	The sally key. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CFacebookManager::GetSallyKey()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	return option->GetPropertyString("sally", "facebookSallyKey", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::Disable()
///
/// \brief	Disables this object. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::Disable()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	option->SetPropertyBool("sally", "facebook", false);
	option->SetPropertyString("sally", "facebookAccessToken", "");
	option->SetPropertyString("sally", "facebookUserName", "");
	option->SetPropertyString("sally", "facebookUserId", "");
	option->SetPropertyString("sally", "facebookSallyKey", "");
	option->SetPropertyString("sally", "facebookLastUpdate", "");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::SetFacebookUserName(const std::string& userName)
///
/// \brief	Sets a user name. 
///
/// \author	Christian Knobloch
/// \date	24.12.2010
///
/// \param	userName	Name of the user. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::SetFacebookUserName(const std::string& userName)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	option->SetPropertyString("sally", "facebookUserName", userName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::Enable(const std::string& accessToken,
/// const std::string& userName, const std::string& userId, const std::string& sallyKey)
///
/// \brief	Enables. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \param	accessToken	The access token. 
/// \param	userName	Name of the user. 
/// \param	userId		Identifier for the user. 
/// \param	sallyKey	The sally key. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::Enable(const std::string& accessToken, const std::string& userName,
							   const std::string& userId)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	option->SetPropertyBool("sally", "facebook", true);
	option->SetPropertyString("sally", "facebookAccessToken", accessToken);
	option->SetPropertyString("sally", "facebookUserName", userName);
	option->SetPropertyString("sally", "facebookUserId", userId);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::GenerateSallyKey()
///
/// \brief	Generates a sally key. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::GenerateSallyKey()
{
	std::string sallyKeyString = SallyAPI::System::SystemHelper::GenerateUniqueID();

	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	option->SetPropertyString("sally", "facebookSallyKey", sallyKeyString);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookManager::ActivateFacebook()
///
/// \brief	Activates a facebook. 
///
/// \author	Christian Knobloch
/// \date	24.12.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFacebookManager::ActivateFacebook()
{
	std::string request = GenerateBaseRequest("fbGetAccessToken");	

	std::string response;
	int byteRead = 0;

	std::string proxy = SallyAPI::System::SallyHelper::GetProxy();
	std::string proxyBypass = SallyAPI::System::SallyHelper::GetProxyBypass();
	SallyAPI::Network::NETWORK_RETURN networkReturn =
		SallyAPI::Network::NetworkHelper::GetHTTPText(COMMUNITY_SERVER, COMMUNITY_PORT,
		request, &byteRead, &response, proxy, proxyBypass);

	switch (networkReturn)
	{
	case SallyAPI::Network::ERROR_PREPARE:
		return false;
	case SallyAPI::Network::ERROR_OPEN:
		return false;
	case SallyAPI::Network::ERROR_HTTP_TIMEOUT:
		return false;
	case SallyAPI::Network::ERROR_NOTHING_READ:
		return false;
	default:
		break;
	}

	if (response.length() == 0)
		return false;

	if (response.find("<error>") != std::string::npos)
		return false;

	std::string tempFile = SallyAPI::Core::CGame::GetMediaFolder();
	tempFile.append("facebookActivate.xml");

	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, response);

	XMLNode xMainNode = XMLNode::parseFile(tempFile.c_str());
	if (xMainNode.isEmpty())
		return false;

	XMLNode root = xMainNode.getChildNode("sallycommunity");
	if (root.isEmpty())
		return false;

	// create
	XMLNode meXML;
	int i = 0;
	do
	{
		meXML = root.getChildNode("me", i);

		if (!meXML.isEmpty())
		{
			std::string userId = CheckForNull(meXML.getAttribute("userID"));
			std::string name = CheckForNull(meXML.getAttribute("name"));
			std::string access = CheckForNull(meXML.getAttribute("access"));

			Enable(access, name, userId);
		}
		++i;
	}
	while (!meXML.isEmpty());

	// cleanup
	DeleteFile(tempFile.c_str());

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookManager::PostMessageToWall(const std::string& message,
/// const std::string& description, const std::string& link, const std::string& image,
/// std::string& errorMessage)
///
/// \brief	Post message to wall. 
///
/// \author	Christian Knobloch
/// \date	24.12.2010
///
/// \param	message					The message. 
/// \param	description				The description. 
/// \param	link					The link. 
/// \param	image					The image. 
/// \param [in,out]	errorMessage	Message describing the error. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFacebookManager::PostMessageToWall(const std::string& message, const std::string& description, 
										 const std::string& link, const std::string& image, 
										 std::string& errorMessage)
{
	if (!IsEnabled())
	{
		errorMessage = "Facebook is not enabled.";
		return false;
	}

	std::string request = GenerateBaseRequest("fbPostWall");

	std::string post = "access_token=";
	post.append(SallyAPI::Network::NetworkHelper::URLEncode(GetFacebookAccessToken()));
	post.append("&message=");
	post.append(SallyAPI::Network::NetworkHelper::URLEncode(message));
	post.append("&link=");
	post.append(SallyAPI::Network::NetworkHelper::URLEncode(link));
	post.append("&description=");
	post.append(SallyAPI::Network::NetworkHelper::URLEncode(description));
	post.append("&image=");
	post.append(SallyAPI::Network::NetworkHelper::URLEncode(image));

	m_tFacebookThread.Request(COMMUNITY_SERVER, COMMUNITY_PORT, request, post);
	m_tFacebookThread.Start();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookManager::ConnectFacebook()
///
/// \brief	Connects the facebook. Requests a token from Facebook.
///
/// \author	Christian Knobloch
/// \date	24.12.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookManager::ConnectFacebook()
{
	GenerateSallyKey();		

	std::string url = "url.dll,FileProtocolHandler https://graph.facebook.com/oauth/authorize?client_id=140915919253462&redirect_uri=";
	url.append("http%3A%2F%2F");
	url.append(COMMUNITY_SERVER);
	url.append("%2FfacebookAccess.php%3FSallyKey%3D");
	url.append(GetSallyKey());
	url.append("&scope=offline_access,publish_stream,read_stream");

	// don't call direct ShellExecute with the URL
	// see sbug ticket 469
	ShellExecute(NULL, "open", "rundll32.exe", url.c_str(), NULL, SW_SHOWNORMAL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CFacebookManager::GetUserImageId(const std::string& userId)
///
/// \brief	Gets a user image identifier. 
///
/// \author	Christian Knobloch
/// \date	27.12.2010
///
/// \param	userId	Identifier for the user. 
///
/// \return	The user image identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CFacebookManager::GetFacebookUserImageId(const std::string& userId)
{
	SallyAPI::System::CAutoLock lock(&m_UserImagesLock);

	if (m_mUserImageId[userId] == NULL)
	{
		m_mUserImageId[userId] = m_iUserImageCounter;

		++m_iUserImageCounter;
	}
	return m_mUserImageId[userId];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookManager::LoadFacebookUserImage(const std::string& userId)
///
/// \brief	Loads a facebook user image. 
///
/// \author	Christian Knobloch
/// \date	27.12.2010
///
/// \param	userId	Identifier for the user. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFacebookManager::LoadFacebookUserImage(const std::string& userId)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CTheme* theme = config->GetTheme();

	std::string imageFile = SallyAPI::Core::CGame::GetMediaFolder();
	imageFile.append("Facebook\\");
	imageFile.append(userId);
	imageFile.append(".jpg");

	int userImageId = GetFacebookUserImageId(userId);

	SallyAPI::System::CAutoLock lock(&m_UserImagesLock);

	theme->RemovePicture(userImageId);

	theme->AddPictureFullPath(imageFile, userImageId, 0);

	return true;
}