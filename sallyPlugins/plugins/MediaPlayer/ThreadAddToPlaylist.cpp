////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\ThreadAddToPlaylist.cpp
///
/// \brief	Implements the thread add to playlist class. 
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

#include "ThreadAddToPlaylist.h"

CThreadAddToPlaylist::CThreadAddToPlaylist()
{
}

CThreadAddToPlaylist::~CThreadAddToPlaylist()
{
}

void CThreadAddToPlaylist::RunEx()
{
	AddToPlaylistFromFilebrowser(m_strFolder);

	m_pSearchWindow->SendMessageToParent(m_pSearchWindow, 0, MS_SALLY_HIDE_WORKING);

	SallyAPI::GUI::SendMessage::CParameterNotificationOSM messageOnScreenMenu(GUI_THEME_SALLY_OSM_ADD, "Added");
	m_pSearchWindow->SendMessageToParent(m_pSearchWindow, 0, MS_SALLY_NOTIFICATION_OSM, &messageOnScreenMenu);
}

void CThreadAddToPlaylist::SetValues(SallyAPI::GUI::CListViewExt* listViewSource, CPlaylist* playlist, SallyAPI::GUI::CForm* searchWindow, const std::string& folder)
{
	m_pListViewSource = listViewSource;
	m_pPlaylist = playlist;
	m_pSearchWindow = searchWindow;
	m_strFolder = folder;
}

void FilewalkerAddFolder(std::string& folder, std::vector<std::string>& folders, std::vector<std::string>& files)
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
					folders.push_back(filename);
				}
				else
				{
					files.push_back(filename);
				}
			}
		} while(FindNextFile(hFile, &FileInformation) == TRUE);
	}
	FindClose(hFile);
	return;
}

void CThreadAddToPlaylist::AddToPlaylistFromFilebrowser(std::string& folder)
{
	folder = SallyAPI::String::PathHelper::CorrectPath(folder);

	std::vector<std::string>	folders;
	std::vector<std::string>	files;

	FilewalkerAddFolder(folder, folders, files);

	std::sort(folders.begin(), folders.end(), SallyAPI::String::StringHelper::StringCompareCaseInsensitivity);
	std::sort(files.begin(), files.end(), SallyAPI::String::StringHelper::StringCompareCaseInsensitivity);

	// iterate folders
	std::vector<std::string>::iterator	foldersIterator = folders.begin();

	while (foldersIterator != folders.end())
	{
		std::string folderName = (*foldersIterator);

		AddToPlaylistFromFilebrowser(folderName);

		++foldersIterator;
	}

		// iterate files
	std::vector<std::string>::iterator	filesIterator = files.begin();

	while (filesIterator != files.end())
	{
		std::string fileName = (*filesIterator);

		if (CAudioFile::IsAudioFile(fileName))
		{
			SallyAPI::GUI::CListViewItem listItem(fileName, SallyAPI::String::PathHelper::GetFileFromPath(fileName), 0);
			m_pPlaylist->AddItem(listItem);
		}
		else if (CVideoFile::IsVideoFile(fileName))
		{
			SallyAPI::GUI::CListViewItem listItem(fileName, SallyAPI::String::PathHelper::GetFileFromPath(fileName), 1);
			m_pPlaylist->AddItem(listItem);
		}

		++filesIterator;
	}
	return;
}