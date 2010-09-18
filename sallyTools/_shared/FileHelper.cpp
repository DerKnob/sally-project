////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	_shared\FileHelper.cpp
///
/// \brief	Implements the file helper class. 
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

#include "FileHelper.h"

using namespace SallyAPI::File;

bool FileHelper::FileExists(const std::string& fileName)
{
	struct stat stFileInfo; 
	bool bReturn; 
	int intStat; 

	// Attempt to get the file attributes 
	intStat = stat(fileName.c_str(),&stFileInfo); 
	if(intStat == 0) { 
		// We were able to get the file attributes 
		// so the file obviously exists. 
		bReturn = true; 
	} else { 
		// We were not able to get the file attributes. 
		// This may mean that we don't have permission to 
		// access the folder which contains this file. If you 
		// need to do that level of checking, lookup the 
		// return values of stat which will give you 
		// more details on why stat failed. 
		bReturn = false; 
	}
	return bReturn;
}


std::vector<std::string> FileHelper::ReadFileToVector(const std::string& file, std::vector<std::string>* addToVector)
{
	std::vector<std::string> v;

	if (addToVector != NULL)
		v.insert(v.begin(), addToVector->begin(), addToVector->end());

	std::string	line;
	HANDLE		hFile;
	DWORD		dwCharRead;
	char		szBuffer[2];	

	hFile = CreateFile(file.c_str(), GENERIC_READ,0,0,OPEN_EXISTING,0,0);

	while((hFile != 0) && (hFile != INVALID_HANDLE_VALUE))
	{
		ReadFile(hFile, &szBuffer,(DWORD) 1, &dwCharRead,NULL);
		if (dwCharRead == 0)
		{
			if (line.length() > 0)
			{
				v.push_back(line);
			}
			CloseHandle(hFile);
			hFile = 0;
			break;
		}
		else if (szBuffer[0] == 10)
		{
			if (line.length() > 0)
			{
				v.push_back(line);
				line.erase();
			}
		}
		else if (szBuffer[0] != 13)
		{
			szBuffer[1] = '\0';
			line.append(szBuffer);
		}
	}
	return v;
}

void FileHelper::AddLineToFile(const std::string &filePath, const std::string &line)
{
	long	fileSizeHigh;
	HANDLE	hFile;
	DWORD	DWord;

	char	lineFeed[3];

	lineFeed[0] = 13;
	lineFeed[1] = 10;
	lineFeed[2] = '\0';

	hFile = CreateFile(filePath.c_str(), GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	fileSizeHigh = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, fileSizeHigh, 0, FILE_BEGIN);

	WriteFile(hFile, line.c_str() ,(DWORD) line.length(), &DWord, 0);
	WriteFile(hFile, lineFeed ,(DWORD) 2, &DWord, 0);
	CloseHandle(hFile);
}