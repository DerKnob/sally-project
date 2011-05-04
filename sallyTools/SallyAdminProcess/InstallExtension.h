////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyAdminProcess\InstallExtension.h
///
/// \brief	Declares the install extension functions. 
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

void SelfDelete(const std::string& installDir)
{
	// Self Delete
	std::string searchFiles = installDir;
	searchFiles = SallyAPI::String::PathHelper::CorrectPath(searchFiles);
	searchFiles.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchFiles.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::string temp = installDir;
			temp.append(folderInformation.cFileName);

			if ((folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				temp = SallyAPI::String::PathHelper::CorrectPath(temp);
				SelfDelete(temp);
				RemoveDirectory(temp.c_str());
			}
			else if ((strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{

				DeleteFile(temp.c_str());
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);	
}

bool InstallExtension(std::string extension, const std::string& installtype)
{
	std::string tempPath = GetWindowsTemp();
	tempPath.append("sallyInstall\\");

	DeleteDirectory(tempPath.c_str(), false); // delete the temp folder
	CreateDirectory(tempPath.c_str(), NULL); // create it new

	std::string sallyInstallPath = SallyAPI::System::SystemHelper::GetModulePath();

	if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallyapplication"))
	{
		sallyInstallPath.append("applications\\");
	}
	else if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallytheme"))
	{
		sallyInstallPath.append("themes\\");
	}
	else if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallylanguage"))
	{
		sallyInstallPath.append("languages\\");
	}
	else if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallykeyboard"))
	{
		sallyInstallPath.append("keyboards\\");
	}
	else if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".zip"))
	{
		sallyInstallPath.append(""); // don't add anything
	}
	else
	{
		return false; // unkown - quite
	}

	std::string folderToDelete;

	/************************************************************************/
	/* Do now the unzip to the temp directory                               */
	/************************************************************************/
	HZIP hz = OpenZip(extension.c_str(),0);

	ZIPENTRY ze;
	GetZipItem(hz, -1, &ze);
	int numitems = ze.index;

	if (numitems == 0)
		return false;

	bool unzipError = false;

	// -1 gives overall information about the zipfile
	for (int zi = 0; zi < numitems; zi++)
	{
		std::string outputFile = tempPath;

		ZIPENTRY ze;
		GetZipItem(hz, zi, &ze);		// fetch individual details

		outputFile.append(ze.name);

		if ((folderToDelete.length() == 0) &&
			(!SallyAPI::String::StringHelper::StringEndsWith(extension, ".zip")))
		{
			if (SallyAPI::String::StringHelper::StringToLower(ze.name).compare("languages/") != 0)
			{
				std::vector<std::string> tok = SallyAPI::String::StringHelper::TokenizeString(ze.name, "/");

				if (tok.size() == 2)
				{
					folderToDelete = sallyInstallPath;
					folderToDelete.append(SallyAPI::String::StringHelper::ReplaceString(ze.name, "/", "\\"));
				}
			}
		}

		// if it is the sallyconfig.exe
		if (SallyAPI::String::StringHelper::StringToLower(outputFile).compare("sallyconfig.exe") == 0)
			outputFile.append(".new");
		if (SallyAPI::String::StringHelper::StringToLower(outputFile).compare("sallyadminprocess.exe") == 0)
			outputFile.append(".new");

		if (!(ze.attr & FILE_ATTRIBUTE_DIRECTORY))
			DeleteFile(outputFile.c_str());

		ZRESULT zres = UnzipItem(hz, zi, outputFile.c_str());		// e.g. the item's name.

		if (zres != ZR_OK)
			unzipError = true;
	}
	CloseZip(hz);

	if (unzipError)
		return false;

	/************************************************************************/
	/* cleanup if we are on beta                                            */
	/************************************************************************/
	if (installtype.compare("installbeta") == 0)
		SelfDelete(SallyAPI::System::SystemHelper::GetModulePath());
	else if (folderToDelete.size() > 0)
	{
		if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallyapplication"))
		{
			DeleteDirectory(folderToDelete.c_str(), false);
		}
		else if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallytheme"))
		{
			DeleteDirectory(folderToDelete.c_str(), false);
		}
		else if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallylanguage"))
		{
			// don't delete the language folder... other programs can have already added some
			// language files. So we should keep it
		}
		else if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallykeyboard"))
		{
			DeleteDirectory(folderToDelete.c_str(), false);
		}
	}

	/************************************************************************/
	/* now move the new unzipped folder to the sally install folder         */
	/************************************************************************/
	std::string newTempPath = tempPath;
	newTempPath.append("*");

	MoveDirectory(newTempPath.c_str(), sallyInstallPath.c_str());

	/************************************************************************/
	/* move now the localistion if we have installed one                    */
	/************************************************************************/
	if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".sallyapplication"))
	{
		std::string newLanguageFolder = sallyInstallPath;
		newLanguageFolder.append("languages");

		std::string installDir = SallyAPI::System::SystemHelper::GetModulePath();

		MoveDirectory(newLanguageFolder.c_str(), installDir.c_str());
	}

	/************************************************************************/
	/* start the self updater                                               */
	/************************************************************************/
	if (SallyAPI::String::StringHelper::StringEndsWith(extension, ".zip"))
	{
		// self update if something to do
		std::string modulePath = SallyAPI::System::SystemHelper::GetModulePath();
		modulePath.append("SallyConfigUpdater.exe");

		ShellExecute(NULL, "open", modulePath.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
	return true;
}