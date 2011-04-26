////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FileHelper.cpp
///
/// \brief	Implements the file helper class. 
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
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "FileHelper.h"

using namespace SallyAPI::File;

bool IsDVD(std::string drive)
{
	drive.erase(drive.length() - 1);

	std::string cs = "\\\\.\\";
	cs.append(drive);

	HANDLE hDrive = CreateFile(cs.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if(hDrive == INVALID_HANDLE_VALUE || GetLastError() != NO_ERROR)
		return false;

	UCHAR buffer[2048]; // Must be big enough hold DEVICE_MEDIA_INFO

	ULONG returned;
	BOOL bStatus = DeviceIoControl(hDrive, IOCTL_STORAGE_GET_MEDIA_TYPES_EX,NULL, 0, &buffer, 2048, &returned, NULL);

	// Close handle. This should work, but if it can't close it something may
	// have gone wrong in the IOCTL call.

	if (!CloseHandle(hDrive) || !bStatus)
		return false;

	PGET_MEDIA_TYPES pMediaTypes = (PGET_MEDIA_TYPES) buffer;
	if(pMediaTypes->DeviceType == FILE_DEVICE_CD_ROM)
		return false;
	else if(pMediaTypes->DeviceType == FILE_DEVICE_DVD)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool FileHelper::FileExists(const std::string& fileName)
///
/// \brief	File exists. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string FileHelper::GetFormatedFileSize(const std::string& fileName)
///
/// \brief	Gets a formated file size. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	The formated file size. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FileHelper::GetFormatedFileSize(const std::string& fileName)
{
	struct stat filestatus;
	stat(fileName.c_str(), &filestatus);

	std::string fileSize;
	if (filestatus.st_size / 1024 < 1)
	{
		fileSize = SallyAPI::String::StringHelper::ConvertToString(filestatus.st_size);
		fileSize.append(" Byte");
	}
	else if (filestatus.st_size / 1024 / 1024 < 1)
	{
		fileSize = SallyAPI::String::StringHelper::ConvertToString(filestatus.st_size / 1024);
		fileSize.append(".");
		if (filestatus.st_size % 1000 > 100)
			fileSize.append(SallyAPI::String::StringHelper::ConvertToString(filestatus.st_size % 1000 / 10));
		else
			fileSize.append("0");
		fileSize.append(" KB");
	}
	else if (filestatus.st_size / 1024 / 1024 / 1024 < 1)
	{
		fileSize = SallyAPI::String::StringHelper::ConvertToString(filestatus.st_size / 1024 / 1024);
		fileSize.append(".");
		if (filestatus.st_size / 1024 % 1000 > 100)
			fileSize.append(SallyAPI::String::StringHelper::ConvertToString(filestatus.st_size / 1024 % 1000 / 10));
		else
			fileSize.append("0");
		fileSize.append(" MB");
	}
	else
	{
		fileSize = SallyAPI::String::StringHelper::ConvertToString(filestatus.st_size / 1024 / 1024 / 1024);
		fileSize.append(".");
		if (filestatus.st_size / 1024 / 1024 % 1000 > 100)
			fileSize.append(SallyAPI::String::StringHelper::ConvertToString(filestatus.st_size / 1024 / 1024 % 1000 / 10));
		else
			fileSize.append("0");
		fileSize.append(" GB");
	}
	return fileSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string FileHelper::GetFormatedFileCreateDate(const std::string& fileName)
///
/// \brief	Gets a formated file create date. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	The formated file create date. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FileHelper::GetFormatedFileCreateDate(const std::string& fileName)
{
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ,0,0,OPEN_EXISTING,0,0);

	if((hFile == 0) || (hFile == INVALID_HANDLE_VALUE))
		return "";


	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC;

	// get file time and date
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
		return "";
	}

	// convert modification time to local time.
	FileTimeToSystemTime(&ftCreate, &stUTC);

	CloseHandle(hFile);

	return SallyAPI::Date::DateHelper::GetDateString(stUTC, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string FileHelper::GetFormatedFileWriteDate(const std::string& fileName)
///
/// \brief	Gets a formated file write date. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	The formated file write date. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FileHelper::GetFormatedFileWriteDate(const std::string& fileName)
{
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ,0,0,OPEN_EXISTING,0,0);

	if((hFile == 0) || (hFile == INVALID_HANDLE_VALUE))
		return "";


	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC;

	// get file time and date
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
		return "";
	}

	// convert modification time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);

	CloseHandle(hFile);

	return SallyAPI::Date::DateHelper::GetDateString(stUTC, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string FileHelper::GetFormatedFileAccessDate(const std::string& fileName)
///
/// \brief	Gets a formated file access date. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	The formated file access date. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FileHelper::GetFormatedFileAccessDate(const std::string& fileName)
{
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ,0,0,OPEN_EXISTING,0,0);

	if((hFile == 0) || (hFile == INVALID_HANDLE_VALUE))
		return "";


	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC;

	// get file time and date
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
		return "";
	}

	// convert modification time to local time.
	FileTimeToSystemTime(&ftAccess, &stUTC);

	CloseHandle(hFile);

	return SallyAPI::Date::DateHelper::GetDateString(stUTC, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool FileHelper::FileExistsAndNotEmpty(const std::string& fileName)
///
/// \brief	File exists and not empty. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileHelper::FileExistsAndNotEmpty(const std::string& fileName)
{
	struct stat stFileInfo; 
	bool bReturn; 
	int intStat; 

	// Attempt to get the file attributes 
	intStat = stat(fileName.c_str(),&stFileInfo); 
	if(intStat == 0) { 
		// We were able to get the file attributes 
		// so the file obviously exists. 
		if (stFileInfo.st_size != 0)
			bReturn = true; 
		else
			bReturn = false;
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string> FileHelper::ReadFileToVector(const std::string& file,
/// std::vector<std::string>* addToVector)
///
/// \brief	Reads a file to vector. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	file				The file. 
/// \param [in,out]	addToVector	If non-null, the add to vector. 
///
/// \return	The file to vector. 
////////////////////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool FileHelper::AddLineToFile(const std::string &filePath, const std::string &line)
///
/// \brief	Adds a line to file to 'line'. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	filePath	Full pathname of the file. 
/// \param	line		The line. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileHelper::AddLineToFile(const std::string &filePath, const std::string &line)
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
		return false;
	}
	fileSizeHigh = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, fileSizeHigh, 0, FILE_BEGIN);

	WriteFile(hFile, line.c_str() ,(DWORD) line.length(), &DWord, 0);
	WriteFile(hFile, lineFeed ,(DWORD) 2, &DWord, 0);
	CloseHandle(hFile);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::vector<std::string, DRIVE_TYPE> FileHelper::GetDriveList()
///
/// \brief	Gets the drive list. 
///
/// \author	Christian Knobloch
/// \date	16.06.2010
///
/// \return	The drive list. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::map<std::string, SallyAPI::File::DRIVE_TYPE> FileHelper::GetDriveList()
{
	std::map<std::string, DRIVE_TYPE> driveList;

	char dir_list[24 * 4 + 1];
	int list_len = GetLogicalDriveStrings(sizeof(dir_list)-1,dir_list); 
	for(int i=0;i<list_len;i+=strlen(dir_list+i)+1) 
	{ 
		UINT type = GetDriveType(dir_list+i); 
		DRIVE_TYPE driveType;
		std::string driveLetter = (char*) dir_list+i;
		switch (type)
		{
		case DRIVE_NO_ROOT_DIR:
			driveType = DRIVE_TYPE_NO_ROOT_DIR;
			break;
		case DRIVE_REMOVABLE:
			driveType = DRIVE_TYPE_REMOVABLE;
			break;
		case DRIVE_FIXED:
			driveType = DRIVE_TYPE_FIXED;
			break;
		case DRIVE_REMOTE:
			driveType = DRIVE_TYPE_REMOTE;
			break;
		case DRIVE_CDROM:
			if (IsDVD(driveLetter))
				driveType = DRIVE_TYPE_DVDROM;
			else
				driveType = DRIVE_TYPE_CDROM;
			break;
		case DRIVE_RAMDISK:
			driveType = DRIVE_TYPE_RAMDISK;
			break;
		default:
			driveType = DRIVE_TYPE_UNKNOWN;
			break;
		}

		driveList[driveLetter] = driveType;
	}
	return driveList;
}