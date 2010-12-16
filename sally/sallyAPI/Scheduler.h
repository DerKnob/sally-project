////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Scheduler.h
///
/// \brief	Declares the scheduler class. 
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
#include "define.h"
#include "AppBase.h"

namespace SallyAPI
{
	namespace Scheduler
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CScheduler
		///
		/// \brief	Scheduler. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CScheduler
		{
		private:
			SallyAPI::GUI::CAppBase*	m_pReporterWindow;
			int							m_iMessageId;
			int							m_iReporterId;
			std::string					m_strIdentifier;
			int							m_iRunEveryDays;
			bool						m_bRunning;
			SYSTEMTIME					m_stStartTime;

			void	ClearStartTime();
		public:
			CScheduler(SallyAPI::GUI::CAppBase* reporterWindow, int reporterId, int messageId,
				const std::string& identifier, int runEveryDays);
			~CScheduler();

			SallyAPI::GUI::CAppBase*	GetReporterWindow();
			std::string	GetIdentifier();
			int			GetReporterId();
			int			GetMessageId();
			int			GetRunEveryDays();
			void		SetRunning(bool running);
			bool		IsRunning();
			SYSTEMTIME	GetStartTime();
		};
	}
}