////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\ImageDatabase.cpp
///
/// \brief	Implements the image database class. 
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

#include "ImageDatabase.h"

void CImageDatabase::GetImagesFromDatabase(SallyAPI::GUI::CAppBase* appBase, std::vector<std::string>* imageList, 
						   const std::string& year, const std::string& month)
{
	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("image.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	bool first = false;

	std::string query;
	query.append("SELECT Filename FROM image ");
	if (year.length() != 0)
	{
		query.append("WHERE ");
		query.append("CreateYear = ");
		query.append(year);
		query.append(" ");
		
		first = true;
	}
	if (month.length() != 0)
	{
		if (!first)
			query.append("WHERE ");
		else
			query.append("AND ");
		query.append("CreateMonth = ");
		query.append(month);
		query.append(" ");
	}
	query.append("GROUP BY Filename ORDER BY Filename;");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sDBFilename = rslt->GetString(1);

			sDBFilename = SallyAPI::String::StringHelper::ReplaceString(sDBFilename, "#", "'");

			imageList->push_back(sDBFilename);
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


void CImageDatabase::FillYearFromDatabase(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CDropDown* dropDown)
{
	dropDown->Clear();

	SallyAPI::GUI::CDropDownItem item("", "All");
	dropDown->AddItem(item);

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("image.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string query;
	query.append("SELECT CreateYear FROM image GROUP BY CreateYear ORDER BY CreateYear");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sYear = rslt->GetString(1);

			SallyAPI::GUI::CDropDownItem item(sYear, sYear);

			dropDown->AddItem(item);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);

	dropDown->SelectItemByIdentifier("");
}

void CImageDatabase::FillMonthFromDatabase(SallyAPI::GUI::CAppBase* appBase, SallyAPI::GUI::CDropDown* dropDown)
{
	dropDown->Clear();

	SallyAPI::GUI::CDropDownItem item("", "All");
	dropDown->AddItem(item);

	std::string mediaDirectory = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	mediaDirectory.append("image.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(mediaDirectory);
	if (!bFileExists)
		return;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(mediaDirectory);

	std::string query;
	query.append("SELECT CreateMonth FROM image GROUP BY CreateMonth ORDER BY CreateMonth");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(query.c_str());

		while (rslt->Next())
		{
			std::string sMonth = rslt->GetString(1);

			SallyAPI::GUI::CDropDownItem item(sMonth, sMonth);

			dropDown->AddItem(item);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(mediaDirectory);

	dropDown->SelectItemByIdentifier("");
}