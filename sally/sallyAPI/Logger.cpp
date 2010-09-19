////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Logger.cpp
///
/// \brief	Implements the logger class. 
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

#include "Logger.h"

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLogger::CLogger(const std::string& fileName, bool createNew, LogLevel logLevel)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	fileName	Filename of the file. 
/// \param	createNew	true to create new. 
/// \param	logLevel	The log level. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CLogger::CLogger(const std::string& fileName, bool createNew, LogLevel logLevel)
{
	m_eLogLevel = logLevel;
	m_strFilename = fileName;

	if (createNew)
	{
		m_hFile = CreateFile(m_strFilename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	}
	else
	{
		m_hFile = CreateFile(m_strFilename.c_str(), GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);

		long fileSizeHigh = GetFileSize(m_hFile, NULL);
		SetFilePointer(m_hFile, fileSizeHigh, 0, FILE_BEGIN);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CLogger::~CLogger()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CLogger::~CLogger()
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFile);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::SetLogLevel(LogLevel logLevel)
///
/// \brief	Sets a log level. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	logLevel	The log level. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::SetLogLevel(LogLevel logLevel)
{
	m_eLogLevel = logLevel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	LogLevel CLogger::GetLogLevel()
///
/// \brief	Gets the log level. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The log level. 
////////////////////////////////////////////////////////////////////////////////////////////////////

LogLevel CLogger::GetLogLevel()
{
	return m_eLogLevel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CLogger::GetFilename()
///
/// \brief	Gets the filename. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The filename. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CLogger::GetFilename()
{
	return m_strFilename;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Error(const std::string& s)
///
/// \brief	Errors. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Error(const std::string& s)
{
	if (m_eLogLevel > LOG_ERROR)
		return;

	std::string log;

	log.append("Error   - ");
	log.append(SallyAPI::Date::DateHelper::GetCurrentDateString());
	log.append(": ");
	log.append(s);

	WriteLog(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Debug(const std::string& s)
///
/// \brief	Debugs. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Debug(const std::string& s)
{
	if (m_eLogLevel > LOG_DEBUG)
		return;

	std::string log;

	log.append("Debug   - ");
	log.append(SallyAPI::Date::DateHelper::GetCurrentDateString());
	log.append(": ");
	log.append(s);

	WriteLog(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Warning(const std::string& s)
///
/// \brief	Warnings. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Warning(const std::string& s)
{
	if (m_eLogLevel > LOG_WARNING)
		return;

	std::string log;

	log.append("Warning - ");
	log.append(SallyAPI::Date::DateHelper::GetCurrentDateString());
	log.append(": ");
	log.append(s);

	WriteLog(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Info(const std::string& s)
///
/// \brief	Infoes. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Info(const std::string& s)
{
	if (m_eLogLevel > LOG_INFO)
		return;

	std::string log;

	log.append("Info    - ");
	log.append(SallyAPI::Date::DateHelper::GetCurrentDateString());
	log.append(": ");
	log.append(s);

	WriteLog(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Error(int s)
///
/// \brief	Errors. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Error(int s)
{
	char	log[12];

	_itoa_s(s, log, 10);

	Error(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Debug(int s)
///
/// \brief	Debugs. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Debug(int s)
{
	char	log[12];

	_itoa_s(s, log, 10);

	Debug(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Warning(int s)
///
/// \brief	Warnings. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Warning(int s)
{
	char	log[12];

	_itoa_s(s, log, 10);

	Warning(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Info(int s)
///
/// \brief	Infoes. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Info(int s)
{
	char	log[12];

	_itoa_s(s, log, 10);

	Info(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Error(HRESULT s)
///
/// \brief	Errors. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Error(HRESULT s)
{
	char	logNo[12];
	_itoa_s(s, logNo, 10);

	std::string log = _com_error(s).ErrorMessage();
	log.append(" - ");
	log.append(logNo);

	Error(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Debug(HRESULT s)
///
/// \brief	Debugs. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Debug(HRESULT s)
{
	char	logNo[12];
	_itoa_s(s, logNo, 10);

	std::string log = _com_error(s).ErrorMessage();
	log.append(" - ");
	log.append(logNo);

	Debug(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Warning(HRESULT s)
///
/// \brief	Warnings. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Warning(HRESULT s)
{
	char	logNo[12];
	_itoa_s(s, logNo, 10);

	std::string log = _com_error(s).ErrorMessage();
	log.append(" - ");
	log.append(logNo);

	Warning(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::Info(HRESULT s)
///
/// \brief	Infoes. 
///
/// \author	Christian Knobloch
/// \date	01.06.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::Info(HRESULT s)
{
	char	logNo[12];
	_itoa_s(s, logNo, 10);

	std::string log = _com_error(s).ErrorMessage();
	log.append(" - ");
	log.append(logNo);

	Info(log);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CLogger::WriteLog(const std::string& s)
///
/// \brief	Writes a log. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	s	The. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CLogger::WriteLog(const std::string& s)
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		DWORD		DWord;

		WriteFile(m_hFile, s.c_str() ,(DWORD) s.length(), &DWord, 0);
		WriteFile(m_hFile, "\r\n",(DWORD) 2, &DWord, 0);
	}
}