////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\CreateNewDatabase.cpp
///
/// \brief	Implements the create new database class. 
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

#include "CreateNewDatabase.h"

CCreateNewDatabase::CCreateNewDatabase()
	: m_pWindow(NULL), m_iMessageId(0), m_iFileCount(0)
{
}

CCreateNewDatabase::~CCreateNewDatabase()
{
}

void CCreateNewDatabase::CollectInformation(std::string& folder)
{
	HANDLE				hFile;
	WIN32_FIND_DATA		FileInformation;

	std::string firstFile;

	folder = SallyAPI::String::PathHelper::CorrectPath(folder);

	firstFile.append(folder);
	firstFile.append("*");

	hFile = FindFirstFile(firstFile.c_str(), &FileInformation);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((strcmp(FileInformation.cFileName, ".") != NULL) &&
				(strcmp(FileInformation.cFileName, "..") != NULL))
			{
				std::string filename;
				filename.append(folder);
				filename.append(FileInformation.cFileName);

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					CollectInformation(filename);
				}
				else  if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpg")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".bmp")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".png")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpeg"))
				{
					m_iFileCount++;
				}
			}
		} while ((FindNextFile(hFile, &FileInformation) == TRUE) && (m_bPleaseStop == false));
	}
	FindClose(hFile);
	return;
}

void CCreateNewDatabase::CreateItem(std::string& sDBFileName, SYSTEMTIME &systemTime)
{
	m_strQueryCreateItem.append("INSERT INTO image ('Filename', 'CreateYear', 'CreateMonth', 'DeleteFlag', 'Rating') ");
	m_strQueryCreateItem.append("VALUES('");
	m_strQueryCreateItem.append(sDBFileName);
	m_strQueryCreateItem.append("',");
	m_strQueryCreateItem.append(SallyAPI::String::StringHelper::ConvertToString(systemTime.wYear));
	m_strQueryCreateItem.append(",");
	m_strQueryCreateItem.append(SallyAPI::String::StringHelper::ConvertToString(systemTime.wMonth));
	m_strQueryCreateItem.append(",0,0);");

	m_iCreateItem++;
}

void CCreateNewDatabase::NoUpdateItem(std::string& sDBFileName) 
{
	m_strQueryNoUpdateItem.append("UPDATE image SET DeleteFlag = 0 ");
	m_strQueryNoUpdateItem.append(" WHERE UPPER(Filename) = UPPER('");
	m_strQueryNoUpdateItem.append(sDBFileName);
	m_strQueryNoUpdateItem.append("');");

	m_iNoUpdateItem++;
}

void CCreateNewDatabase::UpdateItem(std::string& sDBFileName)
{
	m_strQueryUpdateItem.append("UPDATE image SET DeleteFlag = 0 WHERE UPPER(Filename) = UPPER('");
	m_strQueryUpdateItem.append(sDBFileName);
	m_strQueryUpdateItem.append("');");

	m_iUpdateItem++;
}

void CCreateNewDatabase::AddFolder(SallyAPI::Database::CDatabaseConnection* dbconn, std::string& folder)
{
	HANDLE				hFile;
	WIN32_FIND_DATA		FileInformation;

	std::string firstFile;

	folder = SallyAPI::String::PathHelper::CorrectPath(folder);

	firstFile.append(folder);
	firstFile.append("*");

	hFile = FindFirstFile(firstFile.c_str(), &FileInformation);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((strcmp(FileInformation.cFileName, ".") != NULL) &&
				(strcmp(FileInformation.cFileName, "..") != NULL))
			{
				std::string filename;
				filename.append(folder);
				filename.append(FileInformation.cFileName);

				if (FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					AddFolder(dbconn, filename);
				}
				else  if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpg")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".bmp")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".png")
					|| SallyAPI::String::StringHelper::StringEndsWith(filename, ".jpeg"))
				{
					// compare the last file write time with the lastRun time
					FILETIME writeTime = FileInformation.ftLastWriteTime;

					// get file Infos
					std::string sDBFileName = SallyAPI::String::StringHelper::ReplaceString(filename, "'", "#");

					if (CompareFileTime(&m_ftLastRun, &writeTime) <= 0)
					{
						SYSTEMTIME         systemTime;
						ZeroMemory(&systemTime, sizeof(SYSTEMTIME));

						FileTimeToSystemTime(&FileInformation.ftLastWriteTime, &systemTime);

						// check if it exists in the database
						std::string queryFind;
						queryFind.append("SELECT Filename FROM image WHERE UPPER(Filename) = UPPER('");
						queryFind.append(sDBFileName);
						queryFind.append("');");

						dbconn->LockDatabase();
						SallyAPI::Database::CStatement* stmtFind = dbconn->CreateStatement();

						bool found = false;

						try
						{
							SallyAPI::Database::CResultSet* rslt = stmtFind->ExecuteQuery(queryFind.c_str());
							found = rslt->Next();
						}
						catch (SallyAPI::Database::CSQLException* e)
						{
							SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
							logger->Error(e->GetMessage());
						}

						dbconn->ReleaseDatabase();

						if (found)
						{
							UpdateItem(sDBFileName);

							if (m_iUpdateItem == 15)
							{
								ExecuteUpdateItem(dbconn);
							}
						}
						else
						{
							CreateItem(sDBFileName, systemTime);

							if (m_iCreateItem == 15)
							{
								ExecuteCreateItem(dbconn);
							}
						}
					}
					else
					{
						NoUpdateItem(sDBFileName);

						if (m_iNoUpdateItem == 15)
						{
							ExecuteNoUpdateItem(dbconn);
						}
					}

					// update processbar
					m_pProcessbar->SetPosition(m_pProcessbar->GetPosition() + 1);
				}
			}
		} while ((FindNextFile(hFile, &FileInformation) == TRUE) && (m_bPleaseStop == false));
	}
	FindClose(hFile);
	return;
}

void CCreateNewDatabase::ExecuteCreateItem(SallyAPI::Database::CDatabaseConnection* dbconn)
{
	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtInsert = dbconn->CreateStatement();
		stmtInsert->Execute(m_strQueryCreateItem.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	m_strQueryCreateItem = "";
	m_iCreateItem = 0;
}

void CCreateNewDatabase::ExecuteUpdateItem(SallyAPI::Database::CDatabaseConnection* dbconn)
{
	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtFound = dbconn->CreateStatement();
		stmtFound->Execute(m_strQueryUpdateItem.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	m_strQueryUpdateItem = "";
	m_iUpdateItem = 0;
}

void CCreateNewDatabase::ExecuteNoUpdateItem(SallyAPI::Database::CDatabaseConnection* dbconn)
{
	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtFound = dbconn->CreateStatement();
		stmtFound->Execute(m_strQueryNoUpdateItem.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	m_strQueryNoUpdateItem = "";
	m_iNoUpdateItem = 0;
}

void CCreateNewDatabase::RunEx()
{
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

	// reset values
	m_strQueryCreateItem = "";
	m_iCreateItem = 0;
	
	m_strQueryUpdateItem = "";
	m_iUpdateItem = 0;

	m_strQueryNoUpdateItem = "";
	m_iNoUpdateItem = 0;

	// calculate last run time
	// Scheduler
	SallyAPI::Scheduler::CSchedulerManager* schedulerManager = SallyAPI::Scheduler::CSchedulerManager::GetInstance();
	SYSTEMTIME lastRun = schedulerManager->GetLastSchedulerRun(m_pWindow, "dbcreator");

	//Convert both to FILETIMEs
	m_ftLastRun.dwHighDateTime = 0;
	m_ftLastRun.dwLowDateTime = 0;
	SystemTimeToFileTime(&lastRun, &m_ftLastRun);

	std::string mediaDB = m_strMediaDirectory;
	mediaDB.append("image.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDB);

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDB);

	dbconn->LockDatabase();
	if (bFileExists == false)
	{
		// Create
		std::string queryCreate;
		queryCreate.append("CREATE TABLE image ( \
						   Filename     TEXT UNIQUE, \
						   DeleteFlag	INTEGER, \
						   Rating		INTEGER, \
						   CreateYear	INTEGER, \
						   CreateMonth	INTEGER \
						   );");
		try
		{
			SallyAPI::Database::CStatement* stmtCreate = dbconn->CreateStatement();
			stmtCreate->Execute(queryCreate.c_str());
		}
		catch (SallyAPI::Database::CSQLException* e)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error(e->GetMessage());
		}
	}
	dbconn->ReleaseDatabase();

	// Set Delete Flag
	std::string querySetDelete = "UPDATE image SET DeleteFlag = 1;";

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtSetDelete = dbconn->CreateStatement();
		stmtSetDelete->Execute(querySetDelete.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	std::vector<std::string>::iterator iter;

	// Collect Information
	for (iter = m_vFolders.begin(); iter != m_vFolders.end(); iter++)
	{
		std::string folder = *iter;
		CollectInformation(folder);

		if (m_bPleaseStop)
		{
			SallyAPI::Database::CDatabaseConnection::Close(mediaDB);
			return;
		}
	}
	// execute rest of the create item statements
	ExecuteCreateItem(dbconn);
	ExecuteUpdateItem(dbconn);
	ExecuteNoUpdateItem(dbconn);

	m_pProcessbar->SetMaxPosition(m_iFileCount);

	// Create Database
	for (iter = m_vFolders.begin(); iter != m_vFolders.end(); iter++)
	{
		std::string folder = *iter;
		AddFolder(dbconn, folder);

		if (m_bPleaseStop)
		{
			SallyAPI::Database::CDatabaseConnection::Close(mediaDB);
			return;
		}
	}

	// Set Delete Flag
	std::string queryDelete;
	queryDelete.append("DELETE FROM image WHERE DeleteFlag = 1;");

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmtDelete = dbconn->CreateStatement();
		stmtDelete->Execute(queryDelete.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDB);

	m_pWindow->SendMessageToParent(0, 0, m_iMessageId);
}

void CCreateNewDatabase::SetValues(std::vector<std::string>	folders, const std::string& mediaDirectory,
								   int messageId, SallyAPI::GUI::CAppBase* window, SallyAPI::GUI::CProcessbar* processbar)
{
	m_vFolders = folders;
	m_strMediaDirectory = mediaDirectory;
	m_iMessageId = messageId;
	m_pWindow = window;
	m_pProcessbar = processbar;
}
