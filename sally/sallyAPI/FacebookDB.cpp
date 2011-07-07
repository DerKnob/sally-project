////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FacebookDB.cpp
///
/// \brief	Implements the facebook database class. 
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

#include "FacebookDB.h"

#include "SallyApi.h"

#pragma comment(lib, "xmlparser.lib")
#include <xmlParser.h>

SallyAPI::Facebook::CFacebookDB*	SallyAPI::Facebook::CFacebookDB::m_pObject = NULL;

using namespace SallyAPI::Facebook;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFacebookDB::CFacebookDB()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFacebookDB::CFacebookDB()
{
	m_strMediaFolder = SallyAPI::Core::CGame::GetMediaFolder();
	m_strDatabaseFile.append(m_strMediaFolder);
	m_strDatabaseFile.append("facebook.db");

	CheckDatabaseExists();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CFacebookDB::~CFacebookDB()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CFacebookDB::~CFacebookDB()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookDB::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookDB::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Facebook::CFacebookDB* CFacebookDB::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Facebook::CFacebookDB* CFacebookDB::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Facebook::CFacebookDB();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookDB::CheckDatabaseExists()
///
/// \brief	Check database exists. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookDB::CheckDatabaseExists()
{
	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strDatabaseFile);

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strDatabaseFile);

	dbconn->LockDatabase();
	if (bFileExists == false)
	{
		// Create
		std::string queryCreateUser;
		queryCreateUser.append("CREATE TABLE user ( \
								UserID			TEXT UNIQUE, \
								Name			TEXT \
								);");

		try
		{
			SallyAPI::Database::CStatement* stmtCreate = dbconn->CreateStatement();
			stmtCreate->Execute(queryCreateUser.c_str());
		}
		catch (SallyAPI::Database::CSQLException* e)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error(e->GetMessage());
		}

		// Create
		std::string queryCreateStatus;
		queryCreateStatus.append("CREATE TABLE status ( \
								   StatusID			TEXT UNIQUE, \
								   UserID			TEXT, \
								   AppName			TEXT, \
								   ExplicidAppName	TEXT, \
								   Action			TEXT, \
								   ActionName		TEXT, \
								   MessageString	TEXT, \
								   CreateDate		TEXT \
								   );");

		try
		{
			SallyAPI::Database::CStatement* queryCreate = dbconn->CreateStatement();
			queryCreate->Execute(queryCreateStatus.c_str());
		}
		catch (SallyAPI::Database::CSQLException* e)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error(e->GetMessage());
		}
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strDatabaseFile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<SallyAPI::Facebook::CStatusMessage> CFacebookDB::GetLastMessages(int count)
///
/// \brief	Gets the last messages. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	count	Number of. 
///
/// \return	The last messages. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<SallyAPI::Facebook::CStatusMessage> CFacebookDB::GetLastMessages(int count)
{
	std::vector<SallyAPI::Facebook::CStatusMessage> result;

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strDatabaseFile);
	if (!bFileExists)
		return result;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strDatabaseFile);

	// check if it exists in the database
	std::string queryFind;
	queryFind.append("SELECT u.UserID, Name, AppName, ExplicidAppName, Action, ActionName, MessageString, CreateDate FROM status AS s, user AS u WHERE u.UserID = s.UserID ORDER BY CreateDate DESC LIMIT ");
	queryFind.append(SallyAPI::String::StringHelper::ConvertToString(count));
	queryFind.append(";");

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtFind = dbconn->CreateStatement();
		SallyAPI::Database::CResultSet* rslt = stmtFind->ExecuteQuery(queryFind.c_str());
		while (rslt->Next())
		{
			std::string sDBUserId = rslt->GetString(1);
			std::string sDBName = rslt->GetString(2);
			std::string sDBAppName = rslt->GetString(3);
			std::string sDBExplicidAppName = rslt->GetString(4);
			std::string sDBAction = rslt->GetString(5);
			std::string sDBActionName = rslt->GetString(6);
			std::string sDBMessageString = rslt->GetString(7);
			std::string sDBCreateDate = rslt->GetString(8);

			sDBUserId = SallyAPI::String::StringHelper::ReplaceString(sDBUserId, "#", "'");
			sDBName = SallyAPI::String::StringHelper::ReplaceString(sDBName, "#", "'");
			sDBAppName = SallyAPI::String::StringHelper::ReplaceString(sDBAppName, "#", "'");
			sDBExplicidAppName = SallyAPI::String::StringHelper::ReplaceString(sDBExplicidAppName, "#", "'");
			sDBAction = SallyAPI::String::StringHelper::ReplaceString(sDBAction, "#", "'");
			sDBActionName = SallyAPI::String::StringHelper::ReplaceString(sDBActionName, "#", "'");
			sDBMessageString = SallyAPI::String::StringHelper::ReplaceString(sDBMessageString, "#", "'");
			
			SallyAPI::Facebook::CStatusMessage status(sDBCreateDate, sDBUserId, sDBName, sDBExplicidAppName, sDBAppName, sDBAction, sDBActionName, sDBMessageString);
			result.push_back(status);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strDatabaseFile);

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CFacebookDB::GetStatusMessages(SallyAPI::GUI::CGUIBaseObject* mainWindow)
///
/// \brief	Gets the status messages. 
///
/// \author	Christian Knobloch
/// \date	24.12.2010
///
/// \param [in,out]	mainWindow	If non-null, the main window. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CFacebookDB::UpdateStatusMessages(SallyAPI::GUI::CGUIBaseObject* mainWindow)
{
	SallyAPI::Facebook::CFacebookManager* facebookManager = SallyAPI::Facebook::CFacebookManager::GetInstance();
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	CheckDatabaseExists();

	std::string errorMessage;
	std::map<std::string, std::string> requestMap;

	requestMap["lastUpdate"] = option->GetPropertyString("sally", "facebookLastUpdate", "");

	SallyAPI::Network::NETWORK_RETURN errorCode;
	std::string requestResult = facebookManager->RequestData("fbGetFriendsStatusMessages", requestMap, errorMessage, errorCode);

	if (errorCode == SallyAPI::Network::ERROR_NOTHING_READ) // no network connection
		return true;

	if ((errorMessage.length() != 0) || (requestResult.length() == 0))
		return false;

	// image folder
	std::string imageFolder = SallyAPI::Core::CGame::GetMediaFolder();;
	imageFolder.append("Facebook\\");

	CreateDirectory(imageFolder.c_str(), NULL);

	// Update user image
	facebookManager->DownloadFacebookUserImage(imageFolder, facebookManager->GetFacebookUserId());

	// interpret response
	std::string tempFile = SallyAPI::Core::CGame::GetMediaFolder();
	tempFile.append("facebookStatusMessages.xml");

	DeleteFile(tempFile.c_str());

	SallyAPI::File::FileHelper::AddLineToFile(tempFile, requestResult);

	XMLNode xMainNode = XMLNode::openFileHelper(tempFile.c_str());
	if (xMainNode.isEmpty())
		return false;

	XMLNode root = xMainNode.getChildNode("sallycommunity");
	if (root.isEmpty())
		return false;

	// create
	XMLNode friendXML;
	int i = 0;
	do
	{
		friendXML = root.getChildNode("friend", i);

		if (!friendXML.isEmpty())
		{
			std::string userId = CheckForNull(friendXML.getAttribute("userID"));
			std::string name = CheckForNull(friendXML.getAttribute("name"));

			// Update the image
			facebookManager->DownloadFacebookUserImage(imageFolder, userId);

			std::string messageString;
			std::string firstMessageString;
			std::string firstName;
			std::string firstUserId;

			UpdateFriend(userId, name);

			XMLNode message;
			int j = 0;
			do
			{
				message = friendXML.getChildNode("message", j);

				if (!message.isEmpty())
				{
					std::string statusId = CheckForNull(message.getAttribute("statusID"));
					std::string explicidAppName = CheckForNull(message.getAttribute("explicidAppName"));
					std::string appName = CheckForNull(message.getAttribute("appName"));
					std::string action = CheckForNull(message.getAttribute("action"));
					std::string actionName = CheckForNull(message.getAttribute("actionName"));
					std::string createDate = CheckForNull(message.getAttribute("createDate"));
					std::string isNew = CheckForNull(message.getAttribute("isNew"));
					const char* messageChar = message.getText();

					if (messageChar != NULL)
					{
						messageString.erase();
						messageString.append(messageChar);

						explicidAppName = SallyAPI::String::StringHelper::ReplaceString(explicidAppName, "&gt;", ">");
						explicidAppName = SallyAPI::String::StringHelper::ReplaceString(explicidAppName, "&lt;", "<");

						appName = SallyAPI::String::StringHelper::ReplaceString(appName, "&gt;", ">");
						appName = SallyAPI::String::StringHelper::ReplaceString(appName, "&lt;", "<");

						action = SallyAPI::String::StringHelper::ReplaceString(action, "&gt;", ">");
						action = SallyAPI::String::StringHelper::ReplaceString(action, "&lt;", "<");

						actionName = SallyAPI::String::StringHelper::ReplaceString(actionName, "&gt;", ">");
						actionName = SallyAPI::String::StringHelper::ReplaceString(actionName, "&lt;", "<");

						messageString = SallyAPI::String::StringHelper::ReplaceString(messageString, "&gt;", ">");
						messageString = SallyAPI::String::StringHelper::ReplaceString(messageString, "&lt;", "<");

						if (firstName.length() == 0)
						{
							if (isNew.compare("true") == 0)
							{
								firstUserId = userId;
								firstName = name;
								firstMessageString = messageString;
							}
						}

						AddStatus(statusId, userId, messageString, explicidAppName, appName, action, actionName, createDate);
					}
				}
				++j;
			}
			while (!message.isEmpty());

			if (firstName.length() > 0)
			{
				if (option->GetPropertyBool("sally", "showFacebookPopupInfos", true))
				{
					// send the popupinfo
					SallyAPI::GUI::SendMessage::CParameterInfoPopup sendMessageParameterInfoPopup(facebookManager->GetFacebookUserImageId(firstUserId), firstName, firstMessageString);
					mainWindow->SendMessageToParent(mainWindow, 0, MS_SALLY_SHOW_INFO_POPUP, &sendMessageParameterInfoPopup);
				}
			}

			// send update event
			facebookManager->SendUpdateToRegistedNotifier();
		}
		++i;
	}
	while (!friendXML.isEmpty());


	// cleanup
	DeleteFile(tempFile.c_str());

	// save facebookLastDate
	std::string requestDate = root.getAttribute("requestDate");
	option->SetPropertyString("sally", "facebookLastUpdate", requestDate);

	CleanUp();

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookDB::CleanUp()
///
/// \brief	Clean up. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookDB::CleanUp()
{
	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strDatabaseFile);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strDatabaseFile);

	SYSTEMTIME lastWeek;
	GetLocalTime(&lastWeek);

	lastWeek.wMonth = lastWeek.wMonth - 1;

	if (lastWeek.wMonth == 0)
	{
		lastWeek.wMonth = 12;
		lastWeek.wYear = lastWeek.wYear - 1;
	}

	std::string dateString = SallyAPI::Date::DateHelper::GetDateString(lastWeek, false);

	// check if it exists in the database
	std::string queryDelete;
	queryDelete.append("DELETE FROM status WHERE CreateDate < '");
	queryDelete.append(dateString);
	queryDelete.append("';");

	dbconn->LockDatabase();

	bool found = false;
	std::string sDBName;
	std::string sDBImage;

	try
	{
		SallyAPI::Database::CStatement* stmtFind = dbconn->CreateStatement();
		SallyAPI::Database::CResultSet* rslt = stmtFind->ExecuteQuery(queryDelete.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strDatabaseFile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookDB::AddStatus(const std::string& statusId, const std::string& userId,
/// const std::string& messageString, const std::string& explicidAppName,
/// const std::string& appName, const std::string& action, const std::string& actionName,
/// const std::string& createDate)
///
/// \brief	Adds the status. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	statusId		Identifier for the status. 
/// \param	userId			Identifier for the user. 
/// \param	messageString	The message string. 
/// \param	explicidAppName	Name of the explicid application. 
/// \param	appName			Name of the application. 
/// \param	action			The action. 
/// \param	actionName		Name of the action. 
/// \param	createDate		Date of the create. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookDB::AddStatus(const std::string& statusId, const std::string& userId,
							 const std::string& messageString, const std::string& explicidAppName, 
							 const std::string& appName, const std::string& action,
							 const std::string& actionName, const std::string& createDate)
{
	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strDatabaseFile);
	if (!bFileExists)
		return;

	std::string messageStringDB = SallyAPI::String::StringHelper::ReplaceString(messageString, "'", "#");
	std::string explicidAppNameDB = SallyAPI::String::StringHelper::ReplaceString(explicidAppName, "'", "#");
	std::string appNameDB = SallyAPI::String::StringHelper::ReplaceString(appName, "'", "#");
	std::string actionDB = SallyAPI::String::StringHelper::ReplaceString(action, "'", "#");
	std::string actionNameDB = SallyAPI::String::StringHelper::ReplaceString(actionName, "'", "#");
	bool found = false;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strDatabaseFile);

	std::string queryFind;
	queryFind.append("SELECT StatusID FROM status WHERE StatusID = '");
	queryFind.append(statusId);
	queryFind.append("';");

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtFind = dbconn->CreateStatement();
		SallyAPI::Database::CResultSet* rslt = stmtFind->ExecuteQuery(queryFind.c_str());
		while (rslt->Next())
		{
			found = true;
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	if (found)
	{
		SallyAPI::Database::CDatabaseConnection::Close(m_strDatabaseFile);
		return;
	}


	std::string queryInsert;
	queryInsert.append("INSERT INTO status ('StatusID', 'UserID', 'AppName', 'ExplicidAppName', 'Action', 'ActionName', 'MessageString', 'CreateDate') ");
	queryInsert.append("VALUES('");
	queryInsert.append(statusId);
	queryInsert.append("', '");
	queryInsert.append(userId);
	queryInsert.append("', '");
	queryInsert.append(appNameDB);
	queryInsert.append("', '");
	queryInsert.append(explicidAppNameDB);
	queryInsert.append("', '");
	queryInsert.append(actionDB);
	queryInsert.append("', '");
	queryInsert.append(actionNameDB);
	queryInsert.append("', '");
	queryInsert.append(messageStringDB);
	queryInsert.append("', '");
	queryInsert.append(createDate);
	queryInsert.append("');");

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtInsert = dbconn->CreateStatement();
		stmtInsert->Execute(queryInsert.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strDatabaseFile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CFacebookDB::UpdateFriend(const std::string& userId, const std::string& name)
///
/// \brief	Updates a friend. 
///
/// \author	Christian Knobloch
/// \date	23.12.2010
///
/// \param	userId	Identifier for the user. 
/// \param	name	The name. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CFacebookDB::UpdateFriend(const std::string& userId, const std::string& name)
{
	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strDatabaseFile);
	if (!bFileExists)
		return;

	std::string userIdDB = userId;
	std::string nameDB = SallyAPI::String::StringHelper::ReplaceString(name, "'", "#");

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strDatabaseFile);

	// check if it exists in the database
	std::string queryFind;
	queryFind.append("SELECT UserID FROM user WHERE UserID = '");
	queryFind.append(userIdDB);
	queryFind.append("';");

	dbconn->LockDatabase();

	bool found = false;

	try
	{
		SallyAPI::Database::CStatement* stmtFind = dbconn->CreateStatement();
		SallyAPI::Database::CResultSet* rslt = stmtFind->ExecuteQuery(queryFind.c_str());
		while (rslt->Next())
		{
			found = true;
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	if (!found)
	{
		std::string queryInsert;

		queryInsert.append("INSERT INTO user ('UserID', 'Name') ");
		queryInsert.append("VALUES('");
		queryInsert.append(userIdDB);
		queryInsert.append("', '");
		queryInsert.append(nameDB);
		queryInsert.append("');");

		dbconn->LockDatabase();

		try
		{
			SallyAPI::Database::CStatement* stmtInsert = dbconn->CreateStatement();
			stmtInsert->Execute(queryInsert.c_str());
		}
		catch (SallyAPI::Database::CSQLException* e)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error(e->GetMessage());
		}

		dbconn->ReleaseDatabase();
	}
	else
	{
		std::string queryUpdate;

		queryUpdate.append("UPDATE user SET NAME = '");
		queryUpdate.append(nameDB);
		queryUpdate.append("' WHERE UserID = '");
		queryUpdate.append(userIdDB);
		queryUpdate.append("';");

		dbconn->LockDatabase();

		try
		{
			SallyAPI::Database::CStatement* stmtInsert = dbconn->CreateStatement();
			stmtInsert->Execute(queryUpdate.c_str());
		}
		catch (SallyAPI::Database::CSQLException* e)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error(e->GetMessage());
		}

		dbconn->ReleaseDatabase();
	}

	SallyAPI::Database::CDatabaseConnection::Close(m_strDatabaseFile);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string> CFacebookDB::GetFriendIds()
///
/// \brief	Gets the friend identifiers. 
///
/// \author	Christian Knobloch
/// \date	24.12.2010
///
/// \return	The friend identifiers. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> CFacebookDB::GetFriendIds()
{
	std::vector<std::string> result;

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strDatabaseFile);
	if (!bFileExists)
		return result;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strDatabaseFile);

	// check if it exists in the database
	std::string queryFind;
	queryFind.append("SELECT DISTINCT UserID FROM status;");

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtFind = dbconn->CreateStatement();
		SallyAPI::Database::CResultSet* rslt = stmtFind->ExecuteQuery(queryFind.c_str());
		while (rslt->Next())
		{
			std::string userId = rslt->GetString(1);
			userId = SallyAPI::String::StringHelper::ReplaceString(userId, "#", "'");
			
			result.push_back(userId);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strDatabaseFile);

	return result;
}