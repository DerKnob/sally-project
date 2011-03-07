////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Logger.h
///
/// \brief	Declares the logger class. 
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

#pragma once
#include "Define.h"
#include <string>
#include <comdef.h>
#include "DateHelper.h"

namespace SallyAPI
{
	namespace System
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	LogLevel
		///
		/// \brief	Values that represent LogLevel. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum LOG_LEVEL {LOG_LEVEL_DEBUG, LOG_LEVEL_INFO, LOG_LEVEL_WARNING, LOG_LEVEL_ERROR};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CLogger
		///
		/// \brief	Logger. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CLogger  
		{
		private:
			std::string		m_strFilename;
			HANDLE			m_hFile;
			LOG_LEVEL		m_eLogLevel;

			CRITICAL_SECTION		m_critSectLock;
		protected:
			virtual void	WriteLog(const std::string& s);
		public:
			CLogger(const std::string& fileName, bool createNew = true, LOG_LEVEL logLevel = LOG_LEVEL_INFO);
			virtual ~CLogger();

			void		SetLogLevel(LOG_LEVEL logLevel);
			LOG_LEVEL	GetLogLevel();
			std::string	GetFilename();

			void	Debug(const std::string& s);
			void	Error(const std::string& s);
			void	Warning(const std::string& s);
			void	Info(const std::string& s);

			void	Debug(int s);
			void	Error(int s);
			void	Warning(int s);
			void	Info(int s);

			void	Debug(HRESULT s);
			void	Error(HRESULT s);
			void	Warning(HRESULT s);
			void	Info(HRESULT s);
		};
	}
}