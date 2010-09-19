////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\UpdateHelper.cpp
///
/// \brief	Implements the update helper class. 
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

#include "UpdateHelper.h"

std::string GetUpdateFolder()
{
	char szPath[MAX_PATH];
	LPITEMIDLIST lpStartFolder = NULL;
	SHGetSpecialFolderLocation(0, CSIDL_COMMON_DOCUMENTS, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);

	std::string temp = szPath;
	temp.append("\\Sally Project\\");
	CreateDirectory(temp.c_str(), NULL);
	temp.append("updates\\");
	CreateDirectory(temp.c_str(), NULL);

	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	return temp;
}


CUpdateHelper::CUpdateHelper()
{
}

CUpdateHelper::~CUpdateHelper()
{
}

bool CUpdateHelper::UpdatePending()
{
	bool updatePending = false;

	std::string searchUpdates = GetUpdateFolder();

	// Add Plugins
	std::string searchFiles = searchUpdates;
	searchFiles.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		pluginFolderInformation;

	hFolder = FindFirstFile(searchFiles.c_str(), &pluginFolderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(pluginFolderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(pluginFolderInformation.cFileName, "..") != 0)
				&& (strcmp(pluginFolderInformation.cFileName, ".") != 0))
			{
				updatePending = true;
			}
		} while(FindNextFile(hFolder, &pluginFolderInformation) == TRUE);
	}
	FindClose(hFolder);
	return updatePending;
}