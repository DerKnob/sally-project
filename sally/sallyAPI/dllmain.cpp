////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\dllmain.cpp
///
/// \brief	Implements the dllmain function. 
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

#include <tchar.h>
#include <windows.h>
#include <winuser.h>
#include <crtdbg.h>
#include "dllmain.h"

#include "resource.h"

#pragma data_seg ("Shared") 
HWND		hWindow = NULL;
#pragma data_seg ()

HHOOK       hhkHook = NULL;
HINSTANCE   hInstance = NULL;

#pragma comment (linker, "/section:Shared,RWS")

BOOL APIENTRY DllMain(HANDLE hInst, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInstance = (HINSTANCE)hInst;
		break;
	}
	return TRUE;
}

LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		// prevent the message to be handled twice
		if ((lParam & 1073741824) != 1073741824)
		{
			::PostMessage(hWindow, WM_KEYHOOK, wParam, lParam);
		}
	}
	return CallNextHookEx (hhkHook, nCode, wParam, lParam);
} 

DLL_API_SALLY BOOL KHSetupHWND(HWND hParent)
{
	if (hWindow != NULL)
		return FALSE;

	hWindow = hParent;
	return TRUE;
}

DLL_API_SALLY BOOL KHEnableHook()
{ 
	if (hhkHook != NULL)
		return FALSE;

	hhkHook = SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC) ShellProc, hInstance, NULL);

	if (hhkHook == NULL)
		return FALSE;

	return TRUE;
} 

DLL_API_SALLY BOOL KHDisableHook()
{
	if (hhkHook == NULL)
		return FALSE;

	UnhookWindowsHookEx(hhkHook);
		
	hhkHook = NULL;
	return TRUE;
} 

DLL_API_SALLY bool IsKHEnabled()
{
	if (hhkHook == NULL)
		return false;

	return true;
}