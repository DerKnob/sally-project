////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\CreateNewDatabase.h
///
/// \brief	Declares the create new database class. 
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

#pragma once
#include <sallyAPI\sallyAPI.h>
#include <string>
#include <vector>
#include <windows.h>
#include "ImageDatabase.h"

class CCreateNewDatabase :
	public SallyAPI::System::CThread
{
private:
	FILETIME					m_ftLastRun;
	std::string					m_strMediaDirectory;
	std::vector<std::string>	m_vFolders;
	int							m_iMessageId;
	SallyAPI::GUI::CAppBase*	m_pWindow;
	SallyAPI::GUI::CProcessbar* m_pProcessbar;
	int							m_iFileCount;

	void	CollectInformation(std::string& folder);
	void	AddFolder(SallyAPI::Database::CDatabaseConnection* dbconn, std::string& folder);

	void CreateItem(std::string& sDBFileName, SYSTEMTIME &systemTime,
		SallyAPI::Database::CDatabaseConnection* dbconn);
	void UpdateItem(std::string& sDBFileName, SallyAPI::Database::CDatabaseConnection* dbconn);
	void NoUpdateItem(std::string& sDBFileName, SallyAPI::Database::CDatabaseConnection* dbconn);

	virtual void	RunEx();
public:
	CCreateNewDatabase();
	virtual ~CCreateNewDatabase();

	void SetValues(std::vector<std::string>	folders, const std::string& mediaDirectory,
		int messageId, SallyAPI::GUI::CAppBase* window, SallyAPI::GUI::CProcessbar* processbar);
};
