////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\CreateNewDatabase.cpp
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
				else if (CAudioFile::IsAudioFile(filename) || CVideoFile::IsVideoFile(filename))
				{
					m_iFileCount++;
				}
			}
		} while ((FindNextFile(hFile, &FileInformation) == TRUE) && (m_bPleaseStop == false));
	}
	FindClose(hFile);
	return;
}

void CCreateNewDatabase::CreateItem(std::string& filename, std::string& sDBFileName, std::string& sDBFileCreated) 
{
	std::string sDBType = "";
	std::string sDBAlbum = "";
	std::string sDBArtist = "";
	std::string sDBBand = "";
	std::string sDBTitle = "";
	std::string sDBYear = "";
	std::string sDBGenre = "";
	std::string sDBTrack = "";
	std::string sDBAlbumArtist = "";

	FillData(filename, sDBType, sDBAlbum, sDBArtist, sDBBand, sDBTitle, sDBYear, sDBGenre, sDBTrack, sDBAlbumArtist);

	m_strQueryCreateItem.append("INSERT INTO media ('Filename', 'Album', 'Artist', 'Band', 'Title', 'Year', 'Genre', 'Track', 'Type', 'DeleteFlag', 'Rating', 'PlayTime', 'Cover', 'DBAddDate', 'FileCreateDate', 'LastPlayDate', 'AlbumArtist') ");
	m_strQueryCreateItem.append("VALUES('");
	m_strQueryCreateItem.append(sDBFileName);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBAlbum);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBArtist);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBBand);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBTitle);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBYear);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBGenre);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBTrack);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBType);
	m_strQueryCreateItem.append("',0,0,0,0,'");
	m_strQueryCreateItem.append(m_strCreateDate);
	m_strQueryCreateItem.append("','");
	m_strQueryCreateItem.append(sDBFileCreated);
	m_strQueryCreateItem.append("', '','");
	m_strQueryCreateItem.append(sDBAlbumArtist);
	m_strQueryCreateItem.append("');");

	m_iCreateItem++;
}

void CCreateNewDatabase::NoItemUpdate(std::string& sDBFileName) 
{
	m_strQueryNoUpdateItem.append("UPDATE media SET DeleteFlag = 0 ");
	m_strQueryNoUpdateItem.append(" WHERE UPPER(Filename) = UPPER('");
	m_strQueryNoUpdateItem.append(sDBFileName);
	m_strQueryNoUpdateItem.append("');");

	m_iNoUpdateItem++;
}

void CCreateNewDatabase::UpdateItem(std::string& filename, std::string& sDBFileName, std::string& sDBFileCreated)
{
	std::string sDBType = "";
	std::string sDBAlbum = "";
	std::string sDBArtist = "";
	std::string sDBBand = "";
	std::string sDBTitle = "";
	std::string sDBYear = "";
	std::string sDBGenre = "";
	std::string sDBTrack = "";
	std::string sDBAlbumArtist = "";

	FillData(filename, sDBType, sDBAlbum, sDBArtist, sDBBand, sDBTitle, sDBYear, sDBGenre, sDBTrack, sDBAlbumArtist);

	m_strQueryUpdateItem.append("UPDATE media SET DeleteFlag = 0, Album = '");
	m_strQueryUpdateItem.append(sDBAlbum);
	m_strQueryUpdateItem.append("', Artist = '");
	m_strQueryUpdateItem.append(sDBArtist);
	m_strQueryUpdateItem.append("', Band ='");
	m_strQueryUpdateItem.append(sDBBand);
	m_strQueryUpdateItem.append("', Title ='");
	m_strQueryUpdateItem.append(sDBTitle);
	m_strQueryUpdateItem.append("', Year = '");
	m_strQueryUpdateItem.append(sDBYear);
	m_strQueryUpdateItem.append("', Genre = '");
	m_strQueryUpdateItem.append(sDBGenre);
	m_strQueryUpdateItem.append("', Track = '");
	m_strQueryUpdateItem.append(sDBTrack);
	m_strQueryUpdateItem.append("', Type = '");
	m_strQueryUpdateItem.append(sDBType);
	m_strQueryUpdateItem.append("', DBAddDate = '");
	m_strQueryUpdateItem.append(m_strCreateDate);
	m_strQueryUpdateItem.append("', FileCreateDate = '");
	m_strQueryUpdateItem.append(sDBFileCreated);
	m_strQueryUpdateItem.append("', AlbumArtist = '");
	m_strQueryUpdateItem.append(sDBAlbumArtist);
	m_strQueryUpdateItem.append("' WHERE UPPER(Filename) = UPPER('");
	m_strQueryUpdateItem.append(sDBFileName);
	m_strQueryUpdateItem.append("');");

	m_iUpdateItem++;
}

void CCreateNewDatabase::FillData(const std::string& filename, std::string& sDBType, std::string& sDBAlbum,
								  std::string& sDBArtist, std::string& sDBBand, 
								  std::string& sDBTitle, std::string& sDBYear,
								  std::string& sDBGenre, std::string& sDBTrack,
								  std::string& sDBAlbumArtist)
{
	if (SallyAPI::String::StringHelper::StringEndsWith(filename, ".mp3")) {
		MP3FileInfo mp3Info;
		mp3Info.Init(filename);

		sDBAlbum.append(mp3Info.GetSzAlbum());
		sDBArtist.append(mp3Info.GetSzArtist());
		sDBTitle.append(mp3Info.GetSzTitle());
		sDBYear.append(mp3Info.GetSzYear());
		sDBGenre.append(mp3Info.GetSzGenre());
		sDBTrack.append(mp3Info.GetSzTrack());
		sDBBand.append(mp3Info.GetSzBand());
		
		if (mp3Info.GetSzBand().length() > 0)
			sDBAlbumArtist.append(mp3Info.GetSzBand());
		else
			sDBAlbumArtist.append(mp3Info.GetSzArtist());

		mp3Info.Free();
	}

	if (CAudioFile::IsAudioFile(filename)) {
		sDBType = "0";
	} else {
		sDBType = "1";
	}

	sDBType = SallyAPI::String::StringHelper::ReplaceString(sDBType, "'", "#");
	sDBAlbum = SallyAPI::String::StringHelper::ReplaceString(sDBAlbum, "'", "#");
	sDBArtist = SallyAPI::String::StringHelper::ReplaceString(sDBArtist, "'", "#");
	sDBTitle = SallyAPI::String::StringHelper::ReplaceString(sDBTitle, "'", "#");
	sDBYear = SallyAPI::String::StringHelper::ReplaceString(sDBYear, "'", "#");
	sDBGenre = SallyAPI::String::StringHelper::ReplaceString(sDBGenre, "'", "#");
	sDBTrack = SallyAPI::String::StringHelper::ReplaceString(sDBTrack, "'", "#");
	sDBBand = SallyAPI::String::StringHelper::ReplaceString(sDBBand, "'", "#");
	sDBAlbumArtist = SallyAPI::String::StringHelper::ReplaceString(sDBAlbumArtist, "'", "#");	
}

void CCreateNewDatabase::AddFolder(SallyAPI::Database::CDatabaseConnection* dbconn, std::string& folder, 
								  const std::string& mediaDirectory)
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
					AddFolder(dbconn, filename, mediaDirectory);
				}
				else if (CAudioFile::IsAudioFile(filename) || CVideoFile::IsVideoFile(filename))
				{
					// compare the last file write time with the lastRun time
					FILETIME writeTimeTemp = FileInformation.ftLastWriteTime;
					FILETIME creationTimeTemp = FileInformation.ftCreationTime;

					FILETIME writeTime;
					FILETIME creationTime;

					FileTimeToLocalFileTime(&writeTimeTemp, &writeTime);
					FileTimeToLocalFileTime(&creationTimeTemp, &creationTime);

					// get file Infos
					std::string sDBFileName = SallyAPI::String::StringHelper::ReplaceString(filename, "'", "#");

					if ((CompareFileTime(&m_ftLastRun, &writeTime) <= 0) ||
						(CompareFileTime(&m_ftLastRun, &creationTime) <= 0))
					{
						SYSTEMTIME creationTimeSystem;

						FileTimeToSystemTime(&creationTime, &creationTimeSystem);

						std::string sDBFileCreated = SallyAPI::Date::DateHelper::GetDateString(creationTimeSystem, false);

						// check if it exists in the database
						std::string queryFind;
						queryFind.append("SELECT Filename FROM media WHERE UPPER(Filename) = UPPER('");
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
							UpdateItem(filename, sDBFileName, sDBFileCreated);

							if (m_iUpdateItem == 15)
							{
								ExecuteUpdateItem(dbconn);
							}
						}
						else
						{
							CreateItem(filename, sDBFileName, sDBFileCreated);

							if (m_iCreateItem == 15)
							{
								ExecuteCreateItem(dbconn);
							}
						}
					}
					else
					{
						NoItemUpdate(sDBFileName);

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
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_LOWEST);

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

	m_strCreateDate = SallyAPI::Date::DateHelper::GetCurrentDateString(false);

	// set the database version
	m_pWindow->SetPropertyInt("databaseVersion", 4);

	std::string mediaDB = m_strMediaDirectory;
	mediaDB.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDB);

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDB);

	dbconn->LockDatabase();
	if (bFileExists == false)
	{
		// Create
		std::string queryCreate;
		queryCreate.append("CREATE TABLE media ( \
						   Filename			TEXT UNIQUE, \
						   Album			TEXT, \
						   Artist			TEXT, \
						   Band				TEXT, \
						   Title			TEXT, \
						   Year				TEXT, \
						   Genre			TEXT, \
						   Track			TEXT, \
						   Type				varchar(1) NOT NULL, \
						   DeleteFlag		INTEGER, \
						   Rating			INTEGER, \
						   PlayTime			INTEGER, \
						   Cover			INTEGER, \
						   DBAddDate		varchar(19) NOT NULL, \
						   FileCreateDate	varchar(19) NOT NULL, \
						   LastPlayDate		varchar(19) NOT NULL, \
						   AlbumArtist      TEXT, \
						   Field1			TEXT, \
						   Field2			TEXT, \
						   PRIMARY KEY (Filename));");

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
	std::string querySetDelete = "UPDATE media SET DeleteFlag = 1;";

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

	m_pProcessbar->SetMaxPosition(m_iFileCount);

	// Create Database
	for (iter = m_vFolders.begin(); iter != m_vFolders.end(); iter++)
	{
		std::string folder = *iter;
		AddFolder(dbconn, folder, mediaDB);

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

	// Set Delete Flag
	std::string queryDelete;
	queryDelete.append("DELETE FROM media WHERE DeleteFlag = 1;");

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
