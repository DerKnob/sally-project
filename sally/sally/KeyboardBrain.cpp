////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\KeyboardBrain.cpp
///
/// \brief	Implements the keyboard brain class. 
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

#include "KeyboardBrain.h"

CKeyboardBrain::CKeyboardBrain()
{
	m_strMediaDirectory = SallyAPI::Core::CGame::GetMediaFolder();
	m_strMediaDirectory.append("keyboard.db");
}

CKeyboardBrain::~CKeyboardBrain()
{
}

void CKeyboardBrain::SetValues(const std::string& sentence)
{
	m_strSentence = sentence;
}

void CKeyboardBrain::RunEx()
{
	std::vector<std::string> words = SallyAPI::String::StringHelper::TokenizeString(m_strSentence, " ");
	std::vector<std::string>::iterator iter = words.begin();

	while (iter != words.end())
	{
		std::string temp = *iter;

		AddWordToDatabase(temp);

		++iter;
	}

	if (words.size() > 1)
		AddSentenceToDatabase(m_strSentence);
}

void CKeyboardBrain::AddSentenceToDatabase(std::string& sentence)
{
	sentence = SallyAPI::String::StringHelper::ReplaceString(sentence, "'", "#");

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strMediaDirectory);

	// check if it exists in the database
	std::string queryFind;
	queryFind.append("SELECT UsedCount FROM sentences WHERE UPPER(Sentence) = UPPER('");
	queryFind.append(sentence);
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
		// Update existing
		std::string queryUpdate;
		queryUpdate.append("UPDATE sentences SET UsedCount = UsedCount + 1 WHERE UPPER(Sentence) = UPPER('");
		queryUpdate.append(sentence);
		queryUpdate.append("');");

		dbconn->LockDatabase();

		try
		{
			SallyAPI::Database::CStatement* stmtUpdate = dbconn->CreateStatement();
			stmtUpdate->Execute(queryUpdate.c_str());
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
		std::string queryInsert;

		queryInsert.append("INSERT INTO sentences ('Sentence', 'UsedCount') ");
		queryInsert.append("VALUES('");
		queryInsert.append(sentence);
		queryInsert.append("', 0);");

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
	SallyAPI::Database::CDatabaseConnection::Close(m_strMediaDirectory);
}

void CKeyboardBrain::AddWordToDatabase(std::string& word)
{
	word = SallyAPI::String::StringHelper::ReplaceString(word, "'", "#");

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strMediaDirectory);

	// check if it exists in the database
	std::string queryFind;
	queryFind.append("SELECT UsedCount FROM words WHERE UPPER(Word) = UPPER('");
	queryFind.append(word);
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
		// Update existing
		std::string queryUpdate;
		queryUpdate.append("UPDATE words SET UsedCount = UsedCount + 1 WHERE UPPER(Word) = UPPER('");
		queryUpdate.append(word);
		queryUpdate.append("');");

		dbconn->LockDatabase();

		try
		{
			SallyAPI::Database::CStatement* stmtUpdate = dbconn->CreateStatement();
			stmtUpdate->Execute(queryUpdate.c_str());
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
		std::string queryInsert;

		queryInsert.append("INSERT INTO words ('Word', 'UsedCount') ");
		queryInsert.append("VALUES('");
		queryInsert.append(word);
		queryInsert.append("', 0);");

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
	SallyAPI::Database::CDatabaseConnection::Close(m_strMediaDirectory);
}