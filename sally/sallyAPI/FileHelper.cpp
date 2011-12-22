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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool IsDVD(std::string drive)
///
/// \brief	Query if 'drive' is dvd. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
///
/// \param	drive	The drive. 
///
/// \return	true if dvd, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

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
/// \fn	bool FileHelper::FileExists(std::string fileName)
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

bool FileHelper::FileExists(std::string fileName)
{
	struct stat	stFileInfo;
	bool		bReturn;
	int			intStat;

	std::size_t pos = fileName.find_last_of("\\");
	if (pos == fileName.length() - 1)
	{
	    fileName.erase(fileName.length() - 1);
	}

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
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);

	if((hFile == 0) || (hFile == INVALID_HANDLE_VALUE))
		return "";


	FILETIME ftCreate, ftAccess, ftWrite;

	// get file time and date
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
		return "";
	}

	CloseHandle(hFile);

	return FormatFileTime(ftCreate);
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
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);

	int err = GetLastError();

	if((hFile == 0) || (hFile == INVALID_HANDLE_VALUE))
		return "";


	FILETIME ftCreate, ftAccess, ftWrite;

	// get file time and date
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
		return "";
	}

	CloseHandle(hFile);

	return FormatFileTime(ftWrite);
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
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, 0,
		OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, 0);

	if((hFile == 0) || (hFile == INVALID_HANDLE_VALUE))
		return "";


	FILETIME ftCreate, ftAccess, ftWrite;

	// get file time and date
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
	{
		CloseHandle(hFile);
		return "";
	}

	CloseHandle(hFile);

	return FormatFileTime(ftAccess);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string FileHelper::FormatFileTime(FILETIME filetime)
///
/// \brief	Format file time. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
///
/// \param	filetime	The filetime. 
///
/// \return	The formatted file time. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string FileHelper::FormatFileTime(FILETIME filetime)
{
	SYSTEMTIME stUTC, stLocal;

	// convert modification time to local time.
	FileTimeToSystemTime(&filetime, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	return SallyAPI::Date::DateHelper::GetDateString(stLocal, false);
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool FileHelper::IsDirectory(const std::string& fileName)
///
/// \brief	Query if 'fileName' is directory. 
///
/// \author	Christian Knobloch
/// \date	25.06.2011
///
/// \param	fileName	Filename of the file. 
///
/// \return	true if directory, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileHelper::IsDirectory(const std::string& filename)
{
	DWORD attributes = GetFileAttributes(filename.c_str());

	if (attributes & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	long FileHelper::CountFilesInFolder(const std::string& sourcePath, long fileCount)
///
/// \brief	Count files in the given folder. 
///
/// \author	Christian Knobloch
/// \date	22.12.2011
///
/// \param	sourcePath	Full pathname of the source file. 
/// \param	fileCount	Number of files. 
///
/// \return	The total number of files in folder. 
////////////////////////////////////////////////////////////////////////////////////////////////////

long FileHelper::CountFilesInFolder(const std::string& sourcePath, long fileCount) 
{ 
	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;
	std::string			searchString;

	searchString = sourcePath;
	searchString = SallyAPI::String::PathHelper::CorrectPath(searchString);
	searchString.append("*");

	hFolder = FindFirstFile(searchString.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				std::string fullName = SallyAPI::String::PathHelper::CorrectPath(sourcePath);
				fullName.append(folderInformation.cFileName);

				fileCount = CountFilesInFolder(fullName, fileCount);
			}
			else
			{
				fileCount++;
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);

	return fileCount;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool FileHelper::FileAction(SallyAPI::File::CFileActionController& fileActionController,
/// const std::string& sourcePath, const std::string& destinationPath)
///
/// \brief	File action. 
///
/// \author	Christian Knobloch
/// \date	22.12.2011
///
/// \param [in,out]	fileActionController	The file action controller. 
/// \param	sourcePath						Full pathname of the source file. 
/// \param	destinationPath					Full pathname of the destination file. 
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool FileHelper::FileAction(SallyAPI::File::CFileActionController* fileActionController,
						   const std::string& sourcePath, const std::string& destinationPath) 
{
	if (!FileExists(sourcePath))
		return false;

	if (fileActionController->ShouldCancel())
		return false;

	bool ret = true;

	if (IsDirectory(sourcePath))
	{
		if (fileActionController->GetFileAction() != SallyAPI::File::FILE_ACTION_DELETE)
			CreateDirectory(destinationPath.c_str(), NULL);

		// directory
		HANDLE				hFolder;
		WIN32_FIND_DATA		folderInformation;
		std::string			searchString;

		searchString = SallyAPI::String::PathHelper::CorrectPath(sourcePath);
		searchString.append("*");

		hFolder = FindFirstFile(searchString.c_str(), &folderInformation);

		if(hFolder != INVALID_HANDLE_VALUE)
		{
			do
			{
				if ((strcmp(folderInformation.cFileName, "..") != 0)
					&& (strcmp(folderInformation.cFileName, ".") != 0))
				{
					std::string sourcePathNew = SallyAPI::String::PathHelper::CorrectPath(sourcePath);
					sourcePathNew.append(folderInformation.cFileName);

					std::string destinationPathNew;
					if (fileActionController->GetFileAction() != SallyAPI::File::FILE_ACTION_DELETE)
					{
						destinationPathNew = SallyAPI::String::PathHelper::CorrectPath(destinationPath);
						destinationPathNew.append(folderInformation.cFileName);
					}

					ret = FileAction(fileActionController, sourcePathNew, destinationPathNew);
				}
			} while((FindNextFile(hFolder, &folderInformation) == TRUE) && (ret));
		}
		FindClose(hFolder);

		// delete now the empty folder
		if (fileActionController->GetFileAction() != SallyAPI::File::FILE_ACTION_COPY)
			ret = (RemoveDirectory(sourcePath.c_str()) == TRUE);
	}
	else
	{
		fileActionController->SetCurrentFile(sourcePath);

		// file
		if (fileActionController->GetFileAction() == SallyAPI::File::FILE_ACTION_COPY)
			ret = fileActionController->CopyFile(sourcePath.c_str(), destinationPath.c_str());
		if (fileActionController->GetFileAction() == SallyAPI::File::FILE_ACTION_MOVE)
			ret = fileActionController->MoveFile(sourcePath.c_str(), destinationPath.c_str());
		if (fileActionController->GetFileAction() == SallyAPI::File::FILE_ACTION_DELETE)
		{
			ret = (DeleteFile(sourcePath.c_str()) == TRUE);
		}

		fileActionController->SetProcessedCount(fileActionController->GetProcessedCount() + 1);
	}
	return ret;
}