////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\MediaDatabase.cpp
///
/// \brief	Implements the media database class. 
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

#include "MediaDatabase.h"

void CMediaDatabase::GetAlbumsFromDatabase(SallyAPI::GUI::CAppBase* appBase, std::vector<CAlbum*>* albumList,
						   const std::string& artist, const std::string& album, const std::string& genre)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	std::string searchArtist = SallyAPI::String::StringHelper::ReplaceString(artist, "'", "#");
	std::string searchAlbum = SallyAPI::String::StringHelper::ReplaceString(album, "'", "#");
	std::string searchGenre = SallyAPI::String::StringHelper::ReplaceString(genre, "'", "#");

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string query;
	query.append("SELECT Album, AlbumArtist, Filename FROM media WHERE ");
	if (searchAlbum.length() == 0)
	{
		query.append("Album != '' ");
	}
	else
	{
		query.append("Album LIKE '%");
		query.append(searchAlbum);
		query.append("%' ");
	}
	query.append("AND ");
	if (searchArtist.length() == 0)
	{
		query.append("AlbumArtist != '' ");
	}
	else
	{
		query.append("AlbumArtist LIKE '%");
		query.append(searchArtist);
		query.append("%' ");
	}
	if (searchGenre.length() == 0)
	{
	}
	else
	{
		query.append("AND ");

		query.append("Genre LIKE '%");
		query.append(searchGenre);
		query.append("%' ");
	}
	query.append(" GROUP BY UPPER(Album), UPPER(AlbumArtist) ORDER BY UPPER(AlbumArtist), UPPER(Album);");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sDBAlbum = rslt->GetString(1);
			std::string sDBArtist = rslt->GetString(2);
			std::string sDBFilename = rslt->GetString(3);

			sDBAlbum = SallyAPI::String::StringHelper::ReplaceString(sDBAlbum, "#", "'");
			sDBArtist = SallyAPI::String::StringHelper::ReplaceString(sDBArtist, "#", "'");
			sDBFilename = SallyAPI::String::StringHelper::ReplaceString(sDBFilename, "#", "'");

			CAlbum* album = new CAlbum(sDBAlbum, sDBArtist, sDBFilename);

			albumList->push_back(album);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);
}

void CMediaDatabase::GetAlbumsFromDatabaseNotLoaded(SallyAPI::GUI::CAppBase* appBase, std::vector<CAlbum*>* albumList)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string query;
	query.append("SELECT Album, AlbumArtist, Filename FROM media WHERE Album != '' AND Artist != '' GROUP BY UPPER(Album), UPPER(Artist) ORDER BY Artist, Album;");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sDBAlbum = rslt->GetString(1);
			std::string sDBArtist = rslt->GetString(2);
			std::string sDBFilename = rslt->GetString(3);

			sDBAlbum = SallyAPI::String::StringHelper::ReplaceString(sDBAlbum, "#", "'");
			sDBArtist = SallyAPI::String::StringHelper::ReplaceString(sDBArtist, "#", "'");
			sDBFilename = SallyAPI::String::StringHelper::ReplaceString(sDBFilename, "#", "'");

			CAlbum* album = new CAlbum(sDBAlbum, sDBArtist, sDBFilename);

			albumList->push_back(album);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);
}

void CMediaDatabase::GetAlbumTitelsFromDatabase(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CListViewExt* listView,
							   const std::string& album, const std::string& artist)
{
	std::map<int, SallyAPI::GUI::CListViewItem*> albumTrackList;

	listView->Clear();

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string searchAlbum = SallyAPI::String::StringHelper::ReplaceString(album, "'", "#");
	std::string searchArtist = SallyAPI::String::StringHelper::ReplaceString(artist, "'", "#");

	std::string query;
	query.append("SELECT Filename, Type, Artist, Title, Track FROM media WHERE UPPER(AlbumArtist) = UPPER('");
	query.append(searchArtist);
	query.append("') AND UPPER(Album) = UPPER('");
	query.append(searchAlbum);
	query.append("');");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sDBFilename = rslt->GetString(1);
			std::string sDBArtist = rslt->GetString(3);
			std::string sDBTitle = rslt->GetString(4);
			std::string sDBTrack = rslt->GetString(5);

			sDBArtist = SallyAPI::String::StringHelper::ReplaceString(sDBArtist, "#", "'");
			sDBTitle = SallyAPI::String::StringHelper::ReplaceString(sDBTitle, "#", "'");
			sDBFilename = SallyAPI::String::StringHelper::ReplaceString(sDBFilename, "#", "'");
			sDBTrack = SallyAPI::String::StringHelper::ReplaceString(sDBTrack, "#", "'");
			int iDBTrack = SallyAPI::String::StringHelper::ConvertToInt(sDBTrack);

			std::string firstLine;

			if ((sDBTitle.length() != 0) && (sDBArtist.length() != 0))
			{
				firstLine.append(sDBArtist);
				firstLine.append(" - ");
				firstLine.append(sDBTitle);
			}
			else
			{
				firstLine.append(SallyAPI::String::PathHelper::GetFileFromPath(sDBFilename));
			}

			SallyAPI::GUI::CListViewItem* listItem = new SallyAPI::GUI::CListViewItem(sDBFilename);

			if (sDBTrack.length() > 0)
				listItem->SetText(sDBTrack, 0);
			else
				listItem->SetText("-", 0);

			listItem->SetText(firstLine, 1);
			
			listItem->SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 0);

			listItem->SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 0);
			listItem->SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 1);

			listItem->SetSmallFont(true, 0);
			listItem->SetSmallFont(false, 1);
			
			albumTrackList[iDBTrack] = listItem;
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);

	// now add the sorted list to the view
	std::map<int, SallyAPI::GUI::CListViewItem*>::iterator iter = albumTrackList.begin();

	while (iter != albumTrackList.end())
	{
		SallyAPI::GUI::CListViewItem* temp = iter->second;
		
		listView->AddItem(*temp);
		delete temp;
		
		++iter;
	}
}

void CMediaDatabase::SetAlbumInDatabase(SallyAPI::GUI::CAppBase* appBase, const std::string& album,
										const std::string& albumArtist, bool set)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string searchAlbum = SallyAPI::String::StringHelper::ReplaceString(album, "'", "#");
	std::string searchAlbumArtist = SallyAPI::String::StringHelper::ReplaceString(albumArtist, "'", "#");

	std::string query;
	query.append("UPDATE media SET Cover = ");
	
	if (set)
		query.append("1");
	else
		query.append("0");

	query.append(" WHERE UPPER(Album) = UPPER('");
	query.append(searchAlbum);
	query.append("') AND UPPER(AlbumArtist) = UPPER('");
	query.append(searchAlbumArtist);
	query.append("');");

	dbconn->LockDatabase();
	
	try
	{
		SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();
		stmt->Execute(query.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);
}

void CMediaDatabase::SearchInDatabase(const std::string& searchForIn, const std::string& searchType,
									  SallyAPI::GUI::CListViewExt* listView, SallyAPI::GUI::CAppBase* appBase)
{
	listView->Clear();

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchForIn, "'", "#");
	std::vector<std::string> tokens = SallyAPI::String::StringHelper::TokenizeString(searchSearchFor, "%");

	std::string query;

	// search like
	if (tokens.size() != 2)
	{
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE (Artist LIKE '%");
		query.append(searchSearchFor);
		query.append("%' OR Album LIKE '%");
		query.append(searchSearchFor);
		query.append("%' OR Title LIKE '%");
		query.append(searchSearchFor);
		query.append("%' OR Band LIKE '%");
		query.append(searchSearchFor);
		query.append("%' OR Filename LIKE '%");
		query.append(searchSearchFor);
		query.append("%')");
	}
	else
	{
		std::string searchForArtist;
		std::string searchForTitle;

		searchForArtist = tokens.at(0);
		searchForTitle = tokens.at(1);

		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE Artist LIKE '%");
		query.append(searchForArtist);
		query.append("%' AND Title LIKE '%");
		query.append(searchForTitle);
		query.append("%'");
	}

	// add type filter
	if (searchType.compare("music") == 0)
	{
		query.append(" AND Type = 0");
	}
	else if (searchType.compare("video") == 0)
	{
		query.append(" AND Type = 1");
	}
	query.append(";");

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sDBFilename = rslt->GetString(1);
			std::string sDBArtist = rslt->GetString(3);
			std::string sDBTitle = rslt->GetString(4);
			std::string sDBAlbum = rslt->GetString(5);

			sDBArtist = SallyAPI::String::StringHelper::ReplaceString(sDBArtist, "#", "'");
			sDBTitle = SallyAPI::String::StringHelper::ReplaceString(sDBTitle, "#", "'");
			sDBFilename = SallyAPI::String::StringHelper::ReplaceString(sDBFilename, "#", "'");
			sDBAlbum = SallyAPI::String::StringHelper::ReplaceString(sDBAlbum, "#", "'");

			std::string firstLine = "";

			if ((sDBTitle.length() != 0) && (sDBArtist.length() != 0))
			{
				firstLine.append(sDBArtist);
				firstLine.append(" - ");
				firstLine.append(sDBTitle);
			}
			else
			{
				firstLine.append(SallyAPI::String::PathHelper::GetFileFromPath(sDBFilename));
			}

			SallyAPI::GUI::CListViewItem listItem(sDBFilename);

			listItem.SetImageId(GUI_THEME_SALLY_ICON_ADD, 0);

			if (rslt->GetInt(2))
				listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO, 1);
			else
				listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 1);
			listItem.SetText(firstLine, 1);
			listItem.SetText(sDBAlbum, 2);

			listItem.SetSmallFont(true, 0);
			listItem.SetSmallFont(false, 1);
			listItem.SetSmallFont(true, 2);

			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 1);
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 2);

			listView->AddItem(listItem);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);
}

std::vector<std::string> CMediaDatabase::SearchInDatabaseExt(const std::string& query, const std::string& searchForIn, int maxResults,
									  SallyAPI::GUI::CAppBase* appBase)
{
	std::vector<std::string> result;

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return result;

	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchForIn, "'", "#");

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while ((rslt->Next()) && (result.size() < maxResults))
		{
			std::string sDB1 = rslt->GetString(1);

			sDB1 = SallyAPI::String::StringHelper::ReplaceString(sDB1, "#", "'");

			std::vector<std::string>::const_iterator iter = result.end();
			
			if (SallyAPI::String::StringHelper::StringContainsCaseInsensitivity(sDB1, searchSearchFor))
			{
				// check if it is already in the list
				iter = find(result.begin(), result.end(), sDB1);
				if (iter == result.end())
					result.push_back(sDB1);
			}
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);

	return result;
}

std::vector<std::string> CMediaDatabase::SearchInDatabaseAlbum(const std::string& searchForIn, int maxResults,
									  SallyAPI::GUI::CAppBase* appBase)
{
	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchForIn, "'", "#");

	std::string query;

	// search like
	query.append("SELECT Album FROM media WHERE (Album LIKE '%");
	query.append(searchSearchFor);
	query.append("%')");
	query.append(" LIMIT ");
	query.append(SallyAPI::String::StringHelper::ConvertToString(maxResults));
	query.append(";");

	return SearchInDatabaseExt(query, searchForIn, maxResults, appBase);
}

std::vector<std::string> CMediaDatabase::SearchInDatabaseArtist(const std::string& searchForIn, int maxResults,
									  SallyAPI::GUI::CAppBase* appBase)
{
	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchForIn, "'", "#");

	std::string query;

	// search like
	query.append("SELECT Artist FROM media WHERE (Artist LIKE '%");
	query.append(searchSearchFor);
	query.append("%' OR Band LIKE '%");
	query.append(searchSearchFor);
	query.append("%')");
	query.append(" LIMIT ");
	query.append(SallyAPI::String::StringHelper::ConvertToString(maxResults));
	query.append(";");

	return SearchInDatabaseExt(query, searchForIn, maxResults, appBase);
}

std::vector<std::string> CMediaDatabase::SearchInDatabaseGenre(const std::string& searchForIn, int maxResults,
									  SallyAPI::GUI::CAppBase* appBase)
{
	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchForIn, "'", "#");

	std::string query;

	// search like
	query.append("SELECT Genre FROM media WHERE (Genre LIKE '%");
	query.append(searchSearchFor);
	query.append("%')");
	query.append(" LIMIT ");
	query.append(SallyAPI::String::StringHelper::ConvertToString(maxResults));
	query.append(";");

	return SearchInDatabaseExt(query, searchForIn, maxResults, appBase);
}

std::vector<std::string> CMediaDatabase::SearchInDatabaseAll(const std::string& searchForIn, int maxResults,
									  SallyAPI::GUI::CAppBase* appBase)
{
	std::vector<std::string> result;

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return result;

	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchForIn, "'", "#");

	std::string query;

	// search like
	query.append("SELECT Artist, Title, Album FROM media WHERE (Artist LIKE '%");
	query.append(searchSearchFor);
	query.append("%' OR Album LIKE '%");
	query.append(searchSearchFor);
	query.append("%' OR Title LIKE '%");
	query.append(searchSearchFor);
	query.append("%' OR Band LIKE '%");
	query.append(searchSearchFor);
	query.append("%' OR Filename LIKE '%");
	query.append(searchSearchFor);
	query.append("%')");
	query.append(" LIMIT ");
	query.append(SallyAPI::String::StringHelper::ConvertToString(maxResults));
	query.append(";");

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while ((rslt->Next()) && (result.size() < maxResults))
		{
			std::string sDB1 = rslt->GetString(1);
			std::string sDB2 = rslt->GetString(2);
			std::string sDB3 = rslt->GetString(3);

			sDB1 = SallyAPI::String::StringHelper::ReplaceString(sDB1, "#", "'");
			sDB2 = SallyAPI::String::StringHelper::ReplaceString(sDB2, "#", "'");
			sDB3 = SallyAPI::String::StringHelper::ReplaceString(sDB3, "#", "'");

			std::vector<std::string>::const_iterator iter = result.end();
			
			if (SallyAPI::String::StringHelper::StringContainsCaseInsensitivity(sDB1, searchSearchFor))
			{
				// check if it is already in the list
				iter = find(result.begin(), result.end(), sDB1);
				if (iter == result.end())
					result.push_back(sDB1);
			}

			if (SallyAPI::String::StringHelper::StringContainsCaseInsensitivity(sDB2, searchSearchFor))
			{
				// check if it is already in the list
				iter = find(result.begin(), result.end(), sDB2);
				if (iter == result.end())
					result.push_back(sDB2);
			}

			if (SallyAPI::String::StringHelper::StringContainsCaseInsensitivity(sDB3, searchSearchFor))
			{
				// check if it is already in the list
				iter = find(result.begin(), result.end(), sDB3);
				if (iter == result.end())
					result.push_back(sDB3);
			}
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);

	return result;
}

void CMediaDatabase::UpdatePlaytime(const std::string& filename, SallyAPI::GUI::CAppBase* appBase)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string dbFilename = SallyAPI::String::StringHelper::ReplaceString(filename, "'", "#");
	std::string playDate = SallyAPI::Date::DateHelper::GetCurrentDateString(false);

	std::string query;
	query.append("UPDATE media SET PlayTime = PlayTime + 1, LastPlayDate = '");
	query.append(playDate);
	query.append("' WHERE UPPER(Filename) = UPPER('");
	query.append(dbFilename);
	query.append("');");

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();
		stmt->Execute(query.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);
}

int CMediaDatabase::GetPlaytime(const std::string& filename, SallyAPI::GUI::CAppBase* appBase)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return -1;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string dbFilename = SallyAPI::String::StringHelper::ReplaceString(filename, "'", "#");

	std::string query;
	query.append("SELECT PlayTime FROM media WHERE UPPER(Filename) = UPPER('");
	query.append(dbFilename);
	query.append("');");

	int playTime = -1;

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			playTime = rslt->GetInt(1);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);

	return playTime;
}

void CMediaDatabase::UpdateRating(const std::string& filename, SallyAPI::GUI::CAppBase* appBase, int rating)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string dbFilename = SallyAPI::String::StringHelper::ReplaceString(filename, "'", "#");

	std::string query;
	query.append("UPDATE media SET Rating = ");
	query.append(SallyAPI::String::StringHelper::ConvertToString(rating));
	query.append(" WHERE UPPER(Filename) = UPPER('");
	query.append(dbFilename);
	query.append("');");

	dbconn->LockDatabase();

	try
	{
		SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();
		stmt->Execute(query.c_str());
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}

	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);
}

int CMediaDatabase::GetRating(const std::string& filename, SallyAPI::GUI::CAppBase* appBase)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return -1;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string dbFilename = SallyAPI::String::StringHelper::ReplaceString(filename, "'", "#");

	std::string query;
	query.append("SELECT Rating FROM media WHERE UPPER(Filename) = UPPER('");
	query.append(dbFilename);
	query.append("');");

	int rating = -1;

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			rating = rslt->GetInt(1);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);

	return rating;
}

void CMediaDatabase::GetStatisticFromDatabase(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CListViewExt* listView, int type, int advancedType)
{
	listView->Clear();

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("media.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string query;
	switch (type)
	{
	case 0:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE PlayTime != 0 ORDER BY PlayTime DESC LIMIT 200;");
		break;
	case 1:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE PlayTime = 0 ORDER BY Title ASC LIMIT 200;");
		break;
	case 2:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE Rating = 5 ORDER BY Title ASC LIMIT 200;");
		break;
	case 3:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE Rating = 4 ORDER BY Title ASC LIMIT 200;");
		break;
	case 4:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE Rating = 3 ORDER BY Title ASC LIMIT 200;");
		break;
	case 5:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE Rating = 2 ORDER BY Title ASC LIMIT 200;");
		break;
	case 6:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE Rating = 1 ORDER BY Title ASC LIMIT 200;");
		break;
	case 7:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE Rating = 0 ORDER BY Title ASC LIMIT 200;");
		break;
	case 8:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media ORDER BY DBAddDate DESC, Title ASC LIMIT 200;");
		break;
	case 9:
		query.append("SELECT Filename, Type, Artist, Title, Album FROM media WHERE PlayTime > 10 AND ");
		
		SYSTEMTIME dateToConvert;
		SYSTEMTIME currentDate;

		GetSystemTime(&currentDate);

		dateToConvert.wDay = currentDate.wDay;

		dateToConvert.wHour = 0;
		dateToConvert.wMinute = 0;
		dateToConvert.wSecond = 0;

		switch (advancedType)
		{
		case 0:
			if (currentDate.wMonth == 1)
			{
				currentDate.wMonth = 12;
				dateToConvert.wYear = currentDate.wYear - 1;
			}
			else
			{
				dateToConvert.wMonth = currentDate.wMonth - 1;
				dateToConvert.wYear = currentDate.wYear;
			}
			break;
		case 1:
			if (currentDate.wMonth <= 6)
			{
				currentDate.wMonth = 12 - ((currentDate.wMonth - 6) * -1);
				dateToConvert.wYear = currentDate.wYear - 1;
			}
			else
			{
				dateToConvert.wMonth = currentDate.wMonth - 6;
				dateToConvert.wYear = currentDate.wYear;
			}
			break;
		case 2:
			dateToConvert.wMonth = currentDate.wMonth;
			dateToConvert.wYear = currentDate.wYear - 1;
			break;
		case 3:
			dateToConvert.wMonth = currentDate.wMonth;
			dateToConvert.wYear = currentDate.wYear - 2;
			break;
		case 4:
			dateToConvert.wMonth = currentDate.wMonth;
			dateToConvert.wYear = currentDate.wYear - 3;
			break;
		case 5:
			dateToConvert.wMonth = currentDate.wMonth;
			dateToConvert.wYear = currentDate.wYear - 4;
			break;
		case 6:
			dateToConvert.wMonth = currentDate.wMonth;
			dateToConvert.wYear = currentDate.wYear - 5;
			break;
		}
		std::string oderThan = SallyAPI::Date::DateHelper::GetDateString(dateToConvert, false);

		query.append("LastPlayDate < '");
		query.append(oderThan);
		query.append("'");
		query.append(" ORDER BY LastPlayDate ASC LIMIT 200;");
		break;
	}


	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sDBFilename = rslt->GetString(1);
			std::string sDBArtist = rslt->GetString(3);
			std::string sDBTitle = rslt->GetString(4);
			std::string sDBAlbum = rslt->GetString(5);

			sDBArtist = SallyAPI::String::StringHelper::ReplaceString(sDBArtist, "#", "'");
			sDBTitle = SallyAPI::String::StringHelper::ReplaceString(sDBTitle, "#", "'");
			sDBFilename = SallyAPI::String::StringHelper::ReplaceString(sDBFilename, "#", "'");
			sDBAlbum = SallyAPI::String::StringHelper::ReplaceString(sDBAlbum, "#", "'");

			std::string firstLine = "";

			if ((sDBTitle.length() != 0) && (sDBArtist.length() != 0))
			{
				firstLine.append(sDBArtist);
				firstLine.append(" - ");
				firstLine.append(sDBTitle);
			}
			else
			{
				firstLine.append(SallyAPI::String::PathHelper::GetFileFromPath(sDBFilename));
			}

			SallyAPI::GUI::CListViewItem listItem(sDBFilename);

			listItem.SetImageId(GUI_THEME_SALLY_ICON_ADD, 0);

			if (rslt->GetInt(2))
				listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO, 1);
			else
				listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 1);
			listItem.SetText(firstLine, 1);
			listItem.SetText(sDBAlbum, 2);

			listItem.SetSmallFont(true, 0);
			listItem.SetSmallFont(false, 1);
			listItem.SetSmallFont(true, 2);

			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 1);
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE, 2);

			listView->AddItem(listItem);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);
}