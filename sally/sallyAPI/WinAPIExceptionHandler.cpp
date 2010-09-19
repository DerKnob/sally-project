////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\WinAPIExceptionHandler.cpp
///
/// \brief	Implements the window a pi exception handler class. 
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

#include "WinAPIExceptionHandler.h"

using namespace SallyAPI::Exception;

std::string SallyAPI::Exception::CWinAPIExceptionHandler::m_strVersionInfo;
std::string SallyAPI::Exception::CWinAPIExceptionHandler::m_strLogFolder;

struct ExceptionCodeDescr
{
	DWORD code;
	const char* descr;
};

#define CODE_DESCR( c ) { c, #c }

static ExceptionCodeDescr exceptionTable[] =
{   
	CODE_DESCR( EXCEPTION_ACCESS_VIOLATION ),
	CODE_DESCR( EXCEPTION_ARRAY_BOUNDS_EXCEEDED ),
	CODE_DESCR( EXCEPTION_BREAKPOINT ),
	CODE_DESCR( EXCEPTION_DATATYPE_MISALIGNMENT ),
	CODE_DESCR( EXCEPTION_FLT_DENORMAL_OPERAND ),
	CODE_DESCR( EXCEPTION_FLT_DIVIDE_BY_ZERO ),
	CODE_DESCR( EXCEPTION_FLT_INEXACT_RESULT ),
	CODE_DESCR( EXCEPTION_FLT_INVALID_OPERATION ),
	CODE_DESCR( EXCEPTION_FLT_OVERFLOW ),
	CODE_DESCR( EXCEPTION_FLT_STACK_CHECK ),
	CODE_DESCR( EXCEPTION_FLT_UNDERFLOW ),
	CODE_DESCR( EXCEPTION_GUARD_PAGE ),
	CODE_DESCR( EXCEPTION_ILLEGAL_INSTRUCTION ),
	CODE_DESCR( EXCEPTION_IN_PAGE_ERROR ),
	CODE_DESCR( EXCEPTION_INT_DIVIDE_BY_ZERO ),
	CODE_DESCR( EXCEPTION_INT_OVERFLOW ),
	CODE_DESCR( EXCEPTION_INVALID_DISPOSITION ),
	CODE_DESCR( EXCEPTION_NONCONTINUABLE_EXCEPTION ),
	CODE_DESCR( EXCEPTION_PRIV_INSTRUCTION ),
	CODE_DESCR( EXCEPTION_SINGLE_STEP ),
	CODE_DESCR( EXCEPTION_STACK_OVERFLOW )
	// might be extended with compiler-specific knowledge of
	// "C++ Exception" code[s]
};

CWinAPIExceptionHandler::CWinAPIExceptionHandler()
{
}

CWinAPIExceptionHandler::~CWinAPIExceptionHandler()
{
}

void CWinAPIExceptionHandler::SetVersionInfo(const std::string& versionInfo)
{
	m_strVersionInfo = versionInfo;
}

void CWinAPIExceptionHandler::SetLogFolder(const std::string& logFolder)
{
	m_strLogFolder = logFolder;
}

std::string CWinAPIExceptionHandler::GetCurrentTimestamp()
{
	SYSTEMTIME		currentTime;
	char			temp[12];
	std::string		timeString;

	GetLocalTime(&currentTime);

	_itoa_s(currentTime.wYear, (LPSTR) temp, 12, 10);
	timeString.append(temp);

	_itoa_s(currentTime.wMonth, (LPSTR) temp, 12, 10);
	if (currentTime.wMonth < 10)
	{
		timeString.append("0");
	}
	timeString.append(temp);

	_itoa_s(currentTime.wDay, (LPSTR) temp, 12, 10);
	if (currentTime.wDay < 10)
	{
		timeString.append("0");
	}
	timeString.append(temp);
	timeString.append("_");

	_itoa_s(currentTime.wHour, (LPSTR) temp, 12, 10);
	if (currentTime.wHour < 10)
	{
		timeString.append("0");
	}
	timeString.append(temp);

	_itoa_s(currentTime.wMinute, (LPSTR) temp, 12, 10);
	if (currentTime.wMinute < 10)
	{
		timeString.append("0");
	}
	timeString.append(temp);

	_itoa_s(currentTime.wSecond, (LPSTR) temp, 12, 10);
	if (currentTime.wSecond < 10)
	{
		timeString.append("0");
	}
	timeString.append(temp);

	return timeString;
}

void CWinAPIExceptionHandler::DumpExceptCallsStack(std::ostream& output, PCONTEXT contextRecord)
{
	output << "--------------------------------------------\n";

	// Output of Version Infos
	if (m_strVersionInfo.length() != 0)
	{
		output << m_strVersionInfo << "\n";
	}

	output << "System Infos:\n\n";

	SallyAPI::Exception::CExceptionStackWalker sw;
	sw.ShowCallstack(GetCurrentThread(), contextRecord);
	output << sw.GetStack();
}

void CWinAPIExceptionHandler::DumpExceptionDescr(std::ostream& output, DWORD code)
{
	const char* descr = NULL ;
	int n = sizeof(exceptionTable) / sizeof(ExceptionCodeDescr) ;

	for(int i = 0; i < n; ++i)
	{
		if (exceptionTable[i].code == code)
		{
			descr = exceptionTable[i].descr;
			break;
		}
	}
	if (descr)
		output << "\tDescription: " << descr << "\n";
	else
		output << "\tDescription unavailable\n";
}

LONG CWinAPIExceptionHandler::Handler(PEXCEPTION_POINTERS pExcPointers)
{
	LONG		res = 0;

	DWORD exceptionCode = pExcPointers->ExceptionRecord->ExceptionCode;

	char modulepath[MAX_PATH];
	if (m_strLogFolder.length() == 0)
	{
		char *pdest;
		GetModuleFileName (GetModuleHandle (NULL),modulepath,254);
		pdest = strrchr (modulepath,'\\');
		modulepath [pdest - modulepath+1] = '\0';
	}
	else
	{
		strcpy_s(modulepath, m_strLogFolder.c_str());
		if (modulepath[strlen(modulepath) - 1] != '\\')
			strcat_s(modulepath, "\\");
	}
	strcat_s(modulepath, GetCurrentTimestamp().c_str());
	strcat_s(modulepath, ".exception");

	std::fstream output(modulepath, std::ios::out);

	output << "Unhandled Exception\n\tCode: " << exceptionCode;
	output << "\n";

	DumpExceptionDescr(output, exceptionCode);
	DumpExceptCallsStack(output, pExcPointers->ContextRecord);

	return res;
}

void CWinAPIExceptionHandler::InitExecptionWatcher()
{
	::SetUnhandledExceptionFilter(SallyAPI::Exception::CWinAPIExceptionHandler::Handler);
}