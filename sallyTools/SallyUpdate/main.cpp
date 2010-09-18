////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyUpdate\main.cpp
///
/// \brief	Implements the main function. 
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

#if defined _M_IX86 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#elif defined _M_IA64 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#elif defined _M_X64 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#else 
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#endif

#include <windows.h>
#include <shlobj.h>
#include <commctrl.h>
#include <string>

#include <xmlParser.h>
#pragma comment(lib, "xmlparser.lib")

#include "../_shared/SystemHelper.h"
#include "../_shared/StringHelper.h"
#include "../_shared/FileHelper.h"
#include "../_shared/Option.h"
#include "../_shared/ListView.h"
#include "../_shared/NetworkHelper.h"
#include "../_shared/FunctionHelper.h"

#include "resource.h"

#define COMMUNITY_SERVER	"www.sally-project.de"
#define COMMUNITY_PORT		80
#define COMMUNITY_URL		"/"

std::string manifest;
std::string searchUpdates;
std::string searchUpdatesTemp;
std::string newAPIVersion;

#include "VersionHelper.h"
#include "NetworkHelper.h"
#include "PluginsHelper.h"
#include "ThemesHelper.h"
#include "LanguagesHelper.h"
#include "KeyboardsHelper.h"

bool UpdatePending()
{
	bool updatePending = false;

	// Add Plugins
	std::string searchFiles = searchUpdates;
	searchFiles.append("*");

	HANDLE				hFolder;
	WIN32_FIND_DATA		folderInformation;

	hFolder = FindFirstFile(searchFiles.c_str(), &folderInformation);

	if(hFolder != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(folderInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (strcmp(folderInformation.cFileName, "..") != 0)
				&& (strcmp(folderInformation.cFileName, ".") != 0))
			{
				updatePending = true;
			}
		} while(FindNextFile(hFolder, &folderInformation) == TRUE);
	}
	FindClose(hFolder);
	return updatePending;
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine,int iCmdShow)
{
	HANDLE hSem = CreateSemaphore(NULL, 0, 1, "SallyProjectUpdater"); 
	if (hSem) 
	{ 
		if (GetLastError() == ERROR_ALREADY_EXISTS) 
		{ 
			return 0; 
		} 
	}

	manifest = SallyAPI::System::SystemHelper::GetModulePath();
	manifest.append("manifest.ini");

	// get path and config files
	searchUpdates = GetUpdateFolder();
	searchUpdatesTemp = GetUpdateFolderTemp();
	iniFile = GetIniFile();

	// delete old xml file
	std::string tempFile = searchUpdatesTemp;
	tempFile.append("updateInfo.xml");

	DeleteFile(tempFile.c_str());

	// check for pending updates
	if (UpdatePending())
	{
		::CloseHandle(hSem);
		return 0;
	}

	// try to update sally
	newAPIVersion = RequestFile("sally.app",
		SallyAPI::System::COption::GetPropertyStringStatic("info", "version", "", manifest),
		SallyAPI::System::COption::GetPropertyStringStatic("info", "apiVersion", "", manifest));

	if (newAPIVersion.length() == 0)
	{
		::CloseHandle(hSem);
		return 0;
	}

	// if beta than end here...
	if (SallyAPI::System::COption::GetPropertyBoolStatic("sally", "beta", false, iniFile))
	{
		::CloseHandle(hSem);
		return 0;
	}

	// go on with all the plugins
	CheckPlugins();
	CheckThemes();
	CheckLanguages();
	CheckKeyboards();

	::CloseHandle(hSem);

	return 0;
}
