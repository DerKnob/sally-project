////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\CreateNewDatabase.h
///
/// \brief	Declares the create new database class. 
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

#pragma once
#include <sallyAPI\sallyAPI.h>
#include <string>
#include <vector>
#include <windows.h>
#include "MediaDatabase.h"
#include "AudioFile.h"
#include "VideoFile.h"

class CCreateNewDatabase :
	public SallyAPI::System::CThread
{
private:
	FILETIME					m_ftLastRun;
	std::string					m_strMediaDirectory;
	std::vector<std::string>	m_vFolders;
	std::string					m_strCreateDate;
	int							m_iMessageId;
	SallyAPI::GUI::CAppBase*	m_pWindow;
	SallyAPI::GUI::CProcessbar* m_pProcessbar;
	int							m_iFileCount;
	std::string					m_strQueryCreateItem;
	int							m_iCreateItem;
	std::string					m_strQueryUpdateItem;
	int							m_iUpdateItem;
	std::string					m_strQueryNoUpdateItem;
	int							m_iNoUpdateItem;

	void	FillData(const std::string& filename, std::string& sDBType,
		std::string& sDBAlbum, std::string& sDBArtist, std::string& sDBBand,
		std::string& sDBTitle, std::string& sDBYear, std::string& sDBGenre, std::string& sDBTrack,
		std::string& sDBAlbumArtist);
	void	CollectInformation(std::string& folder);
	void	AddFolder(SallyAPI::Database::CDatabaseConnection* dbconn, std::string& folder,
		const std::string& mediaDirectory);

	void CreateItem(std::string& filename, std::string& sDBFileName, std::string& sDBFileCreated);
	void UpdateItem(std::string& filename, std::string& sDBFileName, std::string& sDBFileCreated);
	void NoItemUpdate(std::string& sDBFileName);

	void ExecuteCreateItem(SallyAPI::Database::CDatabaseConnection* dbconn);
	void ExecuteUpdateItem(SallyAPI::Database::CDatabaseConnection* dbconn);
	void ExecuteNoUpdateItem(SallyAPI::Database::CDatabaseConnection* dbconn);

	virtual void	RunEx();
public:
	CCreateNewDatabase();
	virtual ~CCreateNewDatabase();

	void SetValues(std::vector<std::string>	folders, const std::string& mediaDirectory,
		int messageId, SallyAPI::GUI::CAppBase* window, SallyAPI::GUI::CProcessbar* processbar);
};
