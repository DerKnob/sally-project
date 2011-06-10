////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\PlaylistHelper.cpp
///
/// \brief	Implements the playlist helper class. 
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

#include "PlaylistHelper.h"

CPlaylistHelper::CPlaylistHelper()
{
}

CPlaylistHelper::~CPlaylistHelper()
{
}

std::string CPlaylistHelper::GetDefaultPlaylist(SallyAPI::GUI::CAppBase* appBase)
{
	std::string s = SallyAPI::System::SallyHelper::GetMediaDirectory(appBase);
	s.append("default.m3u");

	return s;
}

bool CPlaylistHelper::LoadPlaylist(SallyAPI::GUI::CListViewExt* listViewPlaylist, const std::string& playlistName)
{
	HANDLE	hFile = CreateFile(playlistName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	DWORD	dwCharRead;
	char	szBuffer[2];
	std::string fileName;
	std::string extinfo;

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	while((hFile != 0) && (hFile != INVALID_HANDLE_VALUE))
	{
		ReadFile(hFile, szBuffer, (DWORD) 1, &dwCharRead, NULL);

		if ((szBuffer[0] != '\n') && (dwCharRead != 0))
		{
			if (szBuffer[0] != 13)
			{
				szBuffer[1] = '\0';
				fileName.append(szBuffer);
			}
		}
		else
		{
			if (fileName.length() > 3)
			{
				// is it a comment?
				if (fileName.find("#") == 0)
				{
					if (fileName.find("#EXTINF:") == 0)
					{
						int pos = fileName.find(",");
						if (pos != std::string::npos)
						{
							extinfo = fileName.substr(pos + 1);
						}
					}
					fileName.clear();
					continue;
				}

				std::string firstLine = extinfo;
				if (firstLine.length() == 0)
					firstLine = SallyAPI::String::PathHelper::GetFileFromPath(fileName);

				SallyAPI::GUI::CListViewItem listItem(fileName, "", GUI_THEME_SALLY_ICON_REMOVE);

				listItem.SetText(firstLine, 1);
				listItem.SetType(SallyAPI::GUI::LISTVIEWITEM_TYPE_SORTER, 2);

				if (CAudioFile::IsAudioFile(fileName))
					listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_MP3, 1);
				else
					listItem.SetImageId(GUI_THEME_SALLY_ICON_MIMETYPE_VIDEO, 1);

				listViewPlaylist->AddItem(listItem);
			}

			fileName.clear();
		}
		if (dwCharRead == 0)
		{
			CloseHandle(hFile);
			hFile = 0;
		}
	}
	return true;
}

bool CPlaylistHelper::SavePlaylist(std::vector<SallyAPI::GUI::CListViewItem>& itemList, const std::string& playlistName)
{
	std::string folder = SallyAPI::String::PathHelper::GetDirectoryFromPath(playlistName);
	SallyAPI::String::PathHelper::CreateRecursiveDirectory(folder);

	std::string file = SallyAPI::String::PathHelper::GetFileFromPath(playlistName);
	file = SallyAPI::String::PathHelper::CorrectFileName(file);

	std::string outputFile = folder;
	outputFile.append(file);

	DWORD	dwCharRead;
	HANDLE	hFile = CreateFile(outputFile.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	std::vector<SallyAPI::GUI::CListViewItem>::iterator iter;
	for(iter = itemList.begin(); iter != itemList.end(); iter++)
	{
		SallyAPI::GUI::CListViewItem listItem = *iter;

		std::string extinfo = "#EXTINF:0,";
		extinfo.append(listItem.GetText());

		WriteFile(hFile, extinfo.c_str(),(DWORD) extinfo.length(), &dwCharRead, 0);
		WriteFile(hFile, "\n",(DWORD) 1, &dwCharRead, 0);
		WriteFile(hFile, listItem.GetIdentifier().c_str(),(DWORD) listItem.GetIdentifier().length(), &dwCharRead, 0);
		WriteFile(hFile, "\n",(DWORD) 1, &dwCharRead, 0);
	}
	CloseHandle(hFile);
	return true;
}