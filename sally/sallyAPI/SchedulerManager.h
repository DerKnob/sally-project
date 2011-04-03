////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SchedulerManager.h
///
/// \brief	Declares the scheduler manager class. 
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
#include "Scheduler.h"
#include "AutoLock.h"

namespace SallyAPI
{
	namespace Scheduler
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CSchedulerManager
		///
		/// \brief	Manager for schedulers. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CSchedulerManager
		{
		private:
			static SallyAPI::Scheduler::CSchedulerManager*	m_pObject;
			SallyAPI::System::CCritSection					m_critSect;
			std::vector<SallyAPI::Scheduler::CScheduler>	m_vScheduler;

			void SchedulerCanceled(SallyAPI::Scheduler::CScheduler& scheduler);
			void SchedulerFinished(SallyAPI::Scheduler::CScheduler& scheduler);
			void ExecuteScheduler(SallyAPI::Scheduler::CScheduler& scheduler);
			void ResetScheduler(SallyAPI::Scheduler::CScheduler& scheduler);
			std::string GetLastSchedulerRunAsString(SallyAPI::Scheduler::CScheduler& scheduler);
			SYSTEMTIME GetLastSchedulerRun(SallyAPI::Scheduler::CScheduler& scheduler);

			CSchedulerManager();
			~CSchedulerManager();
		public:
			static SallyAPI::Scheduler::CSchedulerManager*	GetInstance();
			static void	DeleteInstance();

			void AddScheduler(const SallyAPI::Scheduler::CScheduler& scheduler);
			void CheckScheduler();

			void RemoveScheduler(const std::string& explicidAppName, const std::string& identifier);
			void RemoveScheduler(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);

			void ExecuteScheduler(const std::string& explicidAppName, const std::string& identifier);
			void ExecuteScheduler(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);

			void SchedulerFinished(const std::string& explicidAppName, const std::string& identifier);
			void SchedulerFinished(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);

			void SchedulerCanceled(const std::string& explicidAppName, const std::string& identifier);
			void SchedulerCanceled(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);

			void ResetScheduler(const std::string& explicidAppName, const std::string& identifier);
			void ResetScheduler(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);

			void SetSchedulerStatus(const std::string& explicidAppName, const std::string& identifier, SallyAPI::Scheduler::SCHEDULER_STATUS status);
			void SetSchedulerStatus(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier, SallyAPI::Scheduler::SCHEDULER_STATUS status);

			SallyAPI::Scheduler::SCHEDULER_STATUS GetSchedulerStatus(const std::string& explicidAppName, const std::string& identifier);
			SallyAPI::Scheduler::SCHEDULER_STATUS GetSchedulerStatus(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);

			std::string GetLastSchedulerRunAsString(const std::string& explicidAppName, const std::string& identifier);
			std::string GetLastSchedulerRunAsString(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);

			SYSTEMTIME GetLastSchedulerRun(const std::string& explicidAppName, const std::string& identifier);
			SYSTEMTIME GetLastSchedulerRun(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier);
		};
	}
}