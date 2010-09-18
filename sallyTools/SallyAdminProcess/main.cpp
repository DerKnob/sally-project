////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyAdminProcess\main.cpp
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
#include <shellapi.h>
#include <string>
#include <map>

#include "../_shared/unzip.h"

#include "../_shared/Option.h"
#include "../_shared/SystemHelper.h"
#include "../_shared/StringHelper.h"
#include "../_shared/FunctionHelper.h"

#include "InstallMimetypes.h"
#include "InstallExtension.h"
#include "UninstallExtension.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	std::string strExtensionFile = szCmdLine;
	
	//strExtensionFile = "install::C:\\Users\\Public\\Documents\\Sally Project\\updates\\option.zip";
	//strExtensionFile = "install::c:\\Users\\Public\\Documents\\Sally Project\\updates\\eRadio.sallyapplication";

	std::vector<std::string> args = SallyAPI::String::StringHelper::TokenizeString(strExtensionFile, "::");

	if ((args.size() < 1) || (args.size() > 2))
		return 1;

	if (args[0].compare("mimetypes") == 0)
	{
		InstallMimetypes();
		return 0; //success
	}

	if (args.size() != 2)
		return 1;

	if ((args[0].compare("install") == 0) || (args[0].compare("installbeta") == 0))
	{
		if (InstallExtension(args[1], args[0]))
			return 0; //success
		
	}
	else if (args[0].compare("delete") == 0)
	{
		if (UninstallExtension(args[1]))
			return 0; //success
	}
	return 1; // error
}