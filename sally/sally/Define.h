////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Define.h
///
/// \brief	Declares the defines. 
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

#define SALLY_VERSION_NO SallyAPI::System::COption::GetPropertyStringStatic("info", "version", "Unkown", SallyAPI::System::SystemHelper::GetModulePath() + "manifest.ini")
#define SALLY_VERSION	("Sally Project " + SALLY_VERSION_NO)
#define SALLY_DESC		"wibbly-wobbly, timey-wimey stuff"
#define WINDOW_NAME		"Sally Project"

#pragma warning(disable: 4995)
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#pragma warning(disable: 4005)

#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "quartz.lib")
#pragma comment(lib, "shell32.lib" )
#pragma comment(lib, "PowrProf.lib")
#pragma comment(lib, "xmlparser.lib")

// ***************************************************************************** //

#define DLL_IMPORT_SALLY_API			1

#include <sallyAPI\sallyAPI.h>
#include <xmlParser.h>

#define GUIM_START_LOADING				1
#define GUIM_EXIT						2
#define GUIM_LOADING_DONE				3
#define GUIM_SHUTDOWN					4
#define GUIM_HIBERNATE					5
#define GUIM_UPDATE						6

#define WM_ENABLE_KEY_HOOK				WM_USER + 5
#define WM_DISABLE_KEY_HOOK				WM_USER + 6