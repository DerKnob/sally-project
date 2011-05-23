////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\dllmain.h
///
/// \brief	Declares the dllmain function. 
///
/// \author	Christian Knobloch
/// \date	30.04.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "_export.h"

#define WM_KEYHOOK WM_APPCOMMAND

DLL_API_SALLY BOOL KHEnableHook();
DLL_API_SALLY BOOL KHDisableHook();
DLL_API_SALLY BOOL KHSetupHWND(HWND hParent);
DLL_API_SALLY bool IsKHEnabled();