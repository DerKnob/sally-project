////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\WinAPIExceptionHandler.h
///
/// \brief	Declares the window a pi exception handler class. 
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

#pragma once

#include "Define.h"
#include <fstream>
#include <iostream>
#include <eh.h>
#include <process.h>
#include <string>
#include "ExceptionStackWalker.h"

namespace SallyAPI
{
	namespace Exception
	{
		class DLL_API_SALLY CWinAPIExceptionHandler
		{
		private:
			static std::string	m_strLogFolder;
			static std::string	m_strVersionInfo;

			static std::string	GetCurrentTimestamp();
			static void			DumpExceptionDescr(std::ostream& output, DWORD code);
			static void			DumpExceptCallsStack(std::ostream& output, PCONTEXT contextRecord);

			CWinAPIExceptionHandler();
			~CWinAPIExceptionHandler();
		public:
			static LONG CALLBACK	Handler(PEXCEPTION_POINTERS p_info);
			static void				InitExecptionWatcher();
			static void				SetVersionInfo(const std::string& versionInfo);
			static void				SetLogFolder(const std::string& logFolder);
		};
	}
}