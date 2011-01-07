////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\main.cpp
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

#include "Define.h"
#include "resource.h"
#include "SallyApp.h"
#include <Dbt.h>
#include "UpdateHelper.h"

CSallyApp* g_pGame = NULL;
bool showCursor = true;
UINT g_uQueryCancelAutoPlay = 0;

void OnCommandSystemDevicechange(WPARAM wParam)
{
	g_pGame->SendMessageToChilds(MS_SALLY_SYSTEM_DEVICECHANGE, NULL);
}

std::string GetLocalisation(int id, HINSTANCE hInstance)
{
	char szStringBuffer[255];
	LoadString (hInstance, id, szStringBuffer, sizeof(szStringBuffer));

	std::string buffer;
	buffer.append(szStringBuffer);

	return buffer;
}

//The windows message handler
LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINTS	pos;
	
	// windows messages for the main Windows window
	switch(msg)
    {
	case WM_SYSCOMMAND: 
		switch (wParam)
		{
		case SC_SCREENSAVE: 
			return 1;
		case SC_MONITORPOWER:
			if (lParam > 0)
			{
				return 1;
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	// if we could not init the game, just exit
	if (g_pGame == NULL)
		return DefWindowProc(hWnd, msg, wParam, lParam);

	// now the messages which are given to the sally application
	switch(msg)
	{
	case WM_DEVICECHANGE:
		if (wParam == DBT_DEVNODES_CHANGED)
			OnCommandSystemDevicechange(wParam);
		break;
	case WM_LBUTTONDBLCLK:
		pos = MAKEPOINTS(lParam);
		g_pGame->ProcessMouseDoubleClick(pos.x, pos.y);
		break;
	case WM_LBUTTONDOWN:
		SetCapture(hWnd);
		pos = MAKEPOINTS(lParam);
		g_pGame->ProcessMouseDown(pos.x, pos.y);
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		pos = MAKEPOINTS(lParam);
		g_pGame->ProcessMouseUp(pos.x, pos.y);
		break;
	case WM_RBUTTONUP:
		if (showCursor)
		{
			ShowCursor(false);
			showCursor = false;
		}
		else
		{
			ShowCursor(true);
			showCursor = true;
		}
		break;
	case WM_MOUSEMOVE:
		pos = MAKEPOINTS(lParam);
		g_pGame->ProcessMouseMove(pos.x, pos.y);
        break;
	case WM_CHAR:
		g_pGame->CharInputPressed((char) wParam);
		break;
	case WM_KEYDOWN:
		g_pGame->KeyDown((int) wParam);
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case MS_SALLY_NEW_VOICE_COMMAND:
			g_pGame->ProcessVoiceCommand();
			break;
		}
		break;
	// Cancel AutoRun
	default:
        if (!g_uQueryCancelAutoPlay)
        { 
            g_uQueryCancelAutoPlay = RegisterWindowMessage(TEXT("QueryCancelAutoPlay"));
        } 
        if (msg && msg == g_uQueryCancelAutoPlay)
        { 
            return TRUE;
        }
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//Application entry point
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	std::string cmdLine;
	cmdLine.append(szCmdLine);

	if (CUpdateHelper::UpdatePending())
	{
		//MessageBox(0, GetLocalisation(IDS_APP_SALLY_UPDATE, hInst).c_str(), GetLocalisation(IDS_APP_SALLY_UPDATE_CAPTION, hInst).c_str(), MB_YESNO | MB_ICONQUESTION) == IDYES)
		if (cmdLine.compare("skipUpdate") != 0)
		{
			std::string modulePath = SallyAPI::System::SystemHelper::GetModulePath();
			modulePath.append("SallyConfig.exe");

			ShellExecute(NULL, "open", modulePath.c_str(), "update", NULL, SW_SHOWNORMAL);
			return 0;
		}
	}

	HANDLE hSem = CreateSemaphore(NULL, 0, 1, "SallyProject"); 
	if (hSem) 
	{ 
		if (GetLastError() == ERROR_ALREADY_EXISTS) 
		{ 
			MessageBox(0, GetLocalisation(IDS_APP_SALLY_RUNNING, hInst).c_str(),
				GetLocalisation(IDS_APP_SALLY_RUNNING_CAPTION, hInst).c_str(), MB_OK | MB_ICONINFORMATION);

			//Die Semaphore existiert bereits, also beenden 
			return 0; 
		} 
	}

	::CoInitializeEx(NULL, COINIT_MULTITHREADED);

	std::string versionInfo = "Sally Version: ";
	versionInfo.append(SALLY_VERSION);
	versionInfo.append("\nSally API Version: ");
	versionInfo.append(SALLY_API_VERSION);

	// My Documents
	char szPath[MAX_PATH];
	LPITEMIDLIST lpStartFolder = NULL;
	SHGetSpecialFolderLocation(0, CSIDL_MYDOCUMENTS, &lpStartFolder);
	SHGetPathFromIDList(lpStartFolder, szPath);
	std::string logFolder = szPath;
	logFolder.append("\\Sally Project\\");

	// CleanUp
	CoTaskMemFree(lpStartFolder);
	lpStartFolder = NULL;

	CreateDirectory(logFolder.c_str(), NULL);

	SallyAPI::Exception::CWinAPIExceptionHandler::SetVersionInfo(versionInfo);
	SallyAPI::Exception::CWinAPIExceptionHandler::SetLogFolder(logFolder);
	SallyAPI::Exception::CWinAPIExceptionHandler::InitExecptionWatcher();
	
	SallyAPI::Core::CGame::SetMediaFolder(logFolder);

	std::string optionDefaultFile = logFolder;
	optionDefaultFile.append("option.ini");
	SallyAPI::System::COption::SetDefaultFile(optionDefaultFile);

	// Check if the option.ini exists
	bool optionExists = SallyAPI::File::FileHelper::FileExists(optionDefaultFile);

	if (!optionExists)
	{
		std::string modulePath = SallyAPI::System::SystemHelper::GetModulePath();
		modulePath.append("option.ini");

		CopyFile(modulePath.c_str(), optionDefaultFile.c_str(), false);
	}

    WNDCLASSEX wc = {sizeof(WNDCLASSEX), CS_CLASSDC | CS_DBLCLKS, WinProc, 0L, 0L, 
					GetModuleHandle(NULL), LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON)),
					LoadCursor(NULL, IDC_ARROW), GetSysColorBrush(COLOR_BTNFACE), NULL,
					//LoadCursor(hInst, MAKEINTRESOURCE(IDC_MOUSE)), GetSysColorBrush(COLOR_BTNFACE), NULL,
                    WINDOW_NAME, NULL};
	
	RegisterClassEx(&wc);

	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	if (SallyAPI::System::COption::GetPropertyBoolStatic("GraphicDevice", "Fullscreen") || SallyAPI::System::COption::GetPropertyBoolStatic("GraphicDevice", "MultiMonitor"))
		style = WS_POPUP | WS_VISIBLE;

    HWND hWnd = CreateWindow(WINDOW_NAME, WINDOW_NAME, 
						style,
						0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
                        NULL, NULL, wc.hInstance, NULL);

	//Show our window
    ShowWindow(hWnd, SW_SHOWDEFAULT);
    UpdateWindow(hWnd);

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	g_pGame = new CSallyApp();

	if (!SallyAPI::System::COption::GetPropertyIntStatic("sally", "showcursor"))
	{
		ShowCursor(FALSE);
		showCursor = false;
	}

	int iQuit = 0;
	if(g_pGame->Initialise(hWnd, hInst))
    { 
		g_pGame->GameLoop();
		iQuit = g_pGame->GetQuit();
    }
	else
	{
		SafeDelete(g_pGame);
		MessageBox(0, "The Sally Engine could not be started.\nPlease run the Sally Config and select another resolution or display.\nIf that doesn't help, please contact the support.", "An error occurred", MB_OK);
	}
	if (!SallyAPI::System::COption::GetPropertyIntStatic("sally", "showcursor"))
		ShowCursor(TRUE);

	SafeDelete(g_pGame);
    UnregisterClass(WINDOW_NAME, wc.hInstance);

	WSACleanup();

	::CoUninitialize();
	::CloseHandle(hSem);
	
	return 0;
}