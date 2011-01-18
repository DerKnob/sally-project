////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	SallyConfigUpdater\main.cpp
///
/// \brief	Implements the main function. 
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
#include "../_shared/SystemHelper.h"
#include "../_shared/FileHelper.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine,int iCmdShow)
{
	// own Semaphore
	bool running = true;
	HANDLE hSem;
	while (running) 
	{ 
		hSem = CreateSemaphore(NULL, 0, 1, "SallyProject"); 
		if (hSem)
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
			{
				running = true;
				::CloseHandle(hSem);
			}
			else
				running = false;
		}
		else
		{
			running = false;
		}
		Sleep(10);
	}

	Sleep (100);

	{
		std::string exeNew = SallyAPI::System::SystemHelper::GetModulePath();
		exeNew.append("SallyConfig.exe.new");

		// if the new file does not exist just exit
		if (SallyAPI::File::FileHelper::FileExists(exeNew))
		{
			std::string exeOld = SallyAPI::System::SystemHelper::GetModulePath();
			exeOld.append("SallyConfig.exe");

			DeleteFile(exeOld.c_str());
			MoveFile(exeNew.c_str(), exeOld.c_str());
		}
	}

	{
		std::string exeNew = SallyAPI::System::SystemHelper::GetModulePath();
		exeNew.append("SallyAdminProcess.exe.new");

		// if the new file does not exist just exit
		if (SallyAPI::File::FileHelper::FileExists(exeNew))
		{
			std::string exeOld = SallyAPI::System::SystemHelper::GetModulePath();
			exeOld.append("SallyAdminProcess.exe");

			DeleteFile(exeOld.c_str());
			MoveFile(exeNew.c_str(), exeOld.c_str());
		}
	}

	// cleanup
	{
		std::string exeDLL = SallyAPI::System::SystemHelper::GetModulePath();
		exeDLL.append("applications\\de.der-knob.sally.app.recipebook\\cookin4life.dll");

		DeleteFile(exeDLL.c_str());
	}

	::CloseHandle(hSem);
	return 0;
}