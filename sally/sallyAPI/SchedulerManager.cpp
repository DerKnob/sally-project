////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SchedulerManager.cpp
///
/// \brief	Implements the scheduler manager class. 
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

#include "SchedulerManager.h"

SallyAPI::Scheduler::CSchedulerManager*	SallyAPI::Scheduler::CSchedulerManager::m_pObject = NULL;

using namespace SallyAPI::Scheduler;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSchedulerManager::CSchedulerManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSchedulerManager::CSchedulerManager()
{
	InitializeCriticalSection(&m_critSect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSchedulerManager::~CSchedulerManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSchedulerManager::~CSchedulerManager()
{
	DeleteCriticalSection(&m_critSect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::Scheduler::CSchedulerManager* CSchedulerManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::Scheduler::CSchedulerManager* CSchedulerManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::Scheduler::CSchedulerManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::CheckScheduler()
///
/// \brief	Check scheduler. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::CheckScheduler()
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance(); 
	SallyAPI::System::COption* option = config->GetOption();
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	EnterCriticalSection(&m_critSect);
	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler& scheduler = *iter;

		SallyAPI::GUI::CAppBase* control = scheduler.GetReporterWindow();

		SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
		SallyAPI::System::COption* option = optionManager->GetOption(control->GetExplicitAppName());

		// now fill the SYSTEMTIME struct
		std::string schedulerName =  "scheduler#";
		schedulerName.append(scheduler.GetIdentifier());

		SYSTEMTIME currentTime;
		SYSTEMTIME lastRun;

		GetLocalTime(&currentTime);

		lastRun.wYear = option->GetPropertyInt(schedulerName, "YYYY", 1980);
		lastRun.wMonth = option->GetPropertyInt(schedulerName, "MM", 1);
		lastRun.wDay = option->GetPropertyInt(schedulerName, "DD", 1);
		lastRun.wHour = option->GetPropertyInt(schedulerName, "H", 0);
		lastRun.wMinute = option->GetPropertyInt(schedulerName, "M", 0);
		lastRun.wSecond = option->GetPropertyInt(schedulerName, "S", 0);
		lastRun.wDayOfWeek = option->GetPropertyInt(schedulerName, "DW", 0);
		lastRun.wMilliseconds = option->GetPropertyInt(schedulerName, "MS", 0);

		if (lastRun.wYear == 0)
		{
			lastRun.wYear = 1980;
			lastRun.wMonth = 1;
			lastRun.wDay = 1;
		}

		long days = SallyAPI::Date::DateHelper::TimeDifferenceInDays(lastRun, currentTime);

		std::string debug;
		debug.append("Scheduler check: ");
		debug.append(control->GetExplicitAppName());
		debug.append(" ");
		debug.append(schedulerName);
		debug.append(" Time Now: ");
		debug.append(SallyAPI::Date::DateHelper::GetDateString(currentTime, false));
		debug.append(" - Time Last Run: ");
		debug.append(SallyAPI::Date::DateHelper::GetDateString(lastRun, false));
		debug.append(" - Day diff: ");
		debug.append(SallyAPI::String::StringHelper::ConvertToString((int) days));
		logger->Debug(debug);
		if ((days >= scheduler.GetRunEveryDays()) && (!scheduler.IsRunning()))
		{
			ExecuteScheduler(scheduler);
			logger->Debug("Scheduler start");
		}
		else
		{
			logger->Debug("Scheduler skip");
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::AddScheduler(const SallyAPI::Scheduler::CScheduler& scheduler)
///
/// \brief	Adds a scheduler. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	scheduler	The scheduler. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::AddScheduler(const SallyAPI::Scheduler::CScheduler& scheduler)
{
	EnterCriticalSection(&m_critSect);
	m_vScheduler.push_back(scheduler);
	LeaveCriticalSection(&m_critSect);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::RemoveScheduler(const std::string& explicidAppName,
/// const std::string& identifier)
///
/// \brief	Removes a scheduler. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::RemoveScheduler(const std::string& explicidAppName, const std::string& identifier)
{
	EnterCriticalSection(&m_critSect);

	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler scheduler = *iter;

		SallyAPI::GUI::CAppBase* appBase = scheduler.GetReporterWindow();
		std::string ean = appBase->GetExplicitAppName();
		if ((ean.compare(explicidAppName) == 0) && (scheduler.GetIdentifier().compare(identifier) == 0))
		{
			m_vScheduler.erase(iter);
			LeaveCriticalSection(&m_critSect);
			return;
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::RemoveScheduler(SallyAPI::GUI::CAppBase* appBase,
/// const std::string& identifier)
///
/// \brief	Removes a scheduler. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::RemoveScheduler(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier)
{
	RemoveScheduler(appBase->GetExplicitAppName(), identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::ExecuteScheduler(const std::string& explicidAppName,
/// const std::string& identifier)
///
/// \brief	Executes the scheduler operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::ExecuteScheduler(const std::string& explicidAppName, const std::string& identifier)
{
	EnterCriticalSection(&m_critSect);

	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler& scheduler = *iter;

		SallyAPI::GUI::CAppBase* appBase = scheduler.GetReporterWindow();
		std::string ean = appBase->GetExplicitAppName();
		if ((ean.compare(explicidAppName) == 0) && (scheduler.GetIdentifier().compare(identifier) == 0))
		{
			ExecuteScheduler(scheduler);
			LeaveCriticalSection(&m_critSect);
			return;
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::ExecuteScheduler(SallyAPI::GUI::CAppBase* appBase,
/// const std::string& identifier)
///
/// \brief	Executes the scheduler operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::ExecuteScheduler(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier)
{
	ExecuteScheduler(appBase->GetExplicitAppName(), identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::ExecuteScheduler(SallyAPI::Scheduler::CScheduler& scheduler)
///
/// \brief	Executes the scheduler operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	scheduler	The scheduler. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::ExecuteScheduler(SallyAPI::Scheduler::CScheduler& scheduler)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	SallyAPI::GUI::CAppBase* control = scheduler.GetReporterWindow();
	control->SendMessageToParent(control, scheduler.GetReporterId(), scheduler.GetMessageId());

	scheduler.SetRunning(true);

	std::string info = "Executing Scheduler: ";
	info.append(control->GetExplicitAppName());
	info.append(" ");
	info.append(scheduler.GetIdentifier());
	logger->Info(info);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::SchedulerFinished(const std::string& explicidAppName,
/// const std::string& identifier)
///
/// \brief	Scheduler finished. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::SchedulerFinished(const std::string& explicidAppName, const std::string& identifier)
{
	EnterCriticalSection(&m_critSect);

	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler& scheduler = *iter;

		SallyAPI::GUI::CAppBase* appBase = scheduler.GetReporterWindow();
		std::string ean = appBase->GetExplicitAppName();
		if ((ean.compare(explicidAppName) == 0) && (scheduler.GetIdentifier().compare(identifier) == 0))
		{
			SchedulerFinished(scheduler);
			LeaveCriticalSection(&m_critSect);
			return;
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::SchedulerFinished(SallyAPI::GUI::CAppBase* appBase,
/// const std::string& identifier)
///
/// \brief	Scheduler finished. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::SchedulerFinished(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier)
{
	SchedulerFinished(appBase->GetExplicitAppName(), identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::SchedulerFinished(SallyAPI::Scheduler::CScheduler& scheduler)
///
/// \brief	Scheduler finished. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	scheduler	The scheduler. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::SchedulerFinished(SallyAPI::Scheduler::CScheduler& scheduler)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	SallyAPI::GUI::CAppBase* control = scheduler.GetReporterWindow();

	std::string schedulerName =  "scheduler#";
	schedulerName.append(scheduler.GetIdentifier());

	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(control->GetExplicitAppName());

	// current Time
	SYSTEMTIME startTime = scheduler.GetStartTime();

	option->SetPropertyInt(schedulerName, "YYYY", startTime.wYear);
	option->SetPropertyInt(schedulerName, "MM", startTime.wMonth);
	option->SetPropertyInt(schedulerName, "DD", startTime.wDay);
	option->SetPropertyInt(schedulerName, "H", startTime.wHour);
	option->SetPropertyInt(schedulerName, "M", startTime.wMinute);
	option->SetPropertyInt(schedulerName, "S", startTime.wSecond);
	option->SetPropertyInt(schedulerName, "DW", startTime.wDayOfWeek);
	option->SetPropertyInt(schedulerName, "MS", startTime.wMilliseconds);

	scheduler.SetRunning(false);

	std::string info = "Scheduler Finished: ";
	info.append(control->GetExplicitAppName());
	info.append(" ");
	info.append(scheduler.GetIdentifier());
	logger->Info(info);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::SchedulerCanceled(const std::string& explicidAppName,
/// const std::string& identifier)
///
/// \brief	Scheduler canceled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::SchedulerCanceled(const std::string& explicidAppName, const std::string& identifier)
{
	EnterCriticalSection(&m_critSect);

	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler& scheduler = *iter;

		SallyAPI::GUI::CAppBase* appBase = scheduler.GetReporterWindow();
		std::string ean = appBase->GetExplicitAppName();
		if ((ean.compare(explicidAppName) == 0) && (scheduler.GetIdentifier().compare(identifier) == 0))
		{
			SchedulerCanceled(scheduler);
			LeaveCriticalSection(&m_critSect);
			return;
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::SchedulerCanceled(SallyAPI::GUI::CAppBase* appBase,
/// const std::string& identifier)
///
/// \brief	Scheduler canceled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::SchedulerCanceled(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier)
{
	SchedulerCanceled(appBase->GetExplicitAppName(), identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::SchedulerCanceled(SallyAPI::Scheduler::CScheduler& scheduler)
///
/// \brief	Scheduler canceled. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	scheduler	The scheduler. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::SchedulerCanceled(SallyAPI::Scheduler::CScheduler& scheduler)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	SallyAPI::GUI::CAppBase* control = scheduler.GetReporterWindow();

	scheduler.SetRunning(false);

	std::string info = "Scheduler Canceled: ";
	info.append(control->GetExplicitAppName());
	info.append(" ");
	info.append(scheduler.GetIdentifier());
	logger->Info(info);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::ResetScheduler(const std::string& explicidAppName,
/// const std::string& identifier)
///
/// \brief	Resets a scheduler. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::ResetScheduler(const std::string& explicidAppName, const std::string& identifier)
{
	EnterCriticalSection(&m_critSect);

	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler& scheduler = *iter;

		SallyAPI::GUI::CAppBase* appBase = scheduler.GetReporterWindow();
		std::string ean = appBase->GetExplicitAppName();
		if ((ean.compare(explicidAppName) == 0) && (scheduler.GetIdentifier().compare(identifier) == 0))
		{
			ResetScheduler(scheduler);
			LeaveCriticalSection(&m_critSect);
			return;
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);
	return;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::ResetScheduler(SallyAPI::GUI::CAppBase* appBase,
/// const std::string& identifier)
///
/// \brief	Resets a scheduler. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
/// \param	identifier		The identifier. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::ResetScheduler(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier)
{
	ResetScheduler(appBase->GetExplicitAppName(), identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSchedulerManager::ResetScheduler(SallyAPI::Scheduler::CScheduler& scheduler)
///
/// \brief	Resets the scheduler described by scheduler. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	scheduler	The scheduler. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSchedulerManager::ResetScheduler(SallyAPI::Scheduler::CScheduler& scheduler)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	SallyAPI::GUI::CAppBase* control = scheduler.GetReporterWindow();

	std::string schedulerName =  "scheduler#";
	schedulerName.append(scheduler.GetIdentifier());

	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(control->GetExplicitAppName());

	// current Time
	option->SetPropertyInt(schedulerName, "YYYY", 1980);
	option->SetPropertyInt(schedulerName, "MM", 1);
	option->SetPropertyInt(schedulerName, "DD", 1);
	option->SetPropertyInt(schedulerName, "H", 0);
	option->SetPropertyInt(schedulerName, "M", 0);
	option->SetPropertyInt(schedulerName, "S", 0);
	option->SetPropertyInt(schedulerName, "DW", 0);
	option->SetPropertyInt(schedulerName, "MS", 0);

	std::string info = "Scheduler Reseted: ";
	info.append(control->GetExplicitAppName());
	info.append(" ");
	info.append(scheduler.GetIdentifier());
	logger->Info(info);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CSchedulerManager::GetLastSchedulerRunAsString(const std::string& explicidAppName,
/// const std::string& identifier)
///
/// \brief	Gets the last scheduler run as string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	identifier		The identifier. 
///
/// \return	The last scheduler run as string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CSchedulerManager::GetLastSchedulerRunAsString(const std::string& explicidAppName, const std::string& identifier)
{
	EnterCriticalSection(&m_critSect);

	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler& scheduler = *iter;

		SallyAPI::GUI::CAppBase* appBase = scheduler.GetReporterWindow();
		std::string ean = appBase->GetExplicitAppName();
		if ((ean.compare(explicidAppName) == 0) && (scheduler.GetIdentifier().compare(identifier) == 0))
		{
			std::string temp = GetLastSchedulerRunAsString(scheduler);
			LeaveCriticalSection(&m_critSect);
			return temp ;
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CSchedulerManager::GetLastSchedulerRunAsString(SallyAPI::GUI::CAppBase* appBase,
/// const std::string& identifier)
///
/// \brief	Gets the last scheduler run as string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
/// \param	identifier		The identifier. 
///
/// \return	The last scheduler run as string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CSchedulerManager::GetLastSchedulerRunAsString(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier)
{
	return GetLastSchedulerRunAsString(appBase->GetExplicitAppName(), identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CSchedulerManager::GetLastSchedulerRunAsString(SallyAPI::Scheduler::CScheduler& scheduler)
///
/// \brief	Gets the last scheduler run as string. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	scheduler	The scheduler. 
///
/// \return	The last scheduler run as string. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CSchedulerManager::GetLastSchedulerRunAsString(SallyAPI::Scheduler::CScheduler& scheduler)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	SallyAPI::GUI::CAppBase* control = scheduler.GetReporterWindow();

	std::string schedulerName =  "scheduler#";
	schedulerName.append(scheduler.GetIdentifier());

	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(control->GetExplicitAppName());

	// current Time
	SYSTEMTIME lastRun;

	lastRun.wYear = option->GetPropertyInt(schedulerName, "YYYY", 1980);
	lastRun.wMonth = option->GetPropertyInt(schedulerName, "MM", 1);
	lastRun.wDay = option->GetPropertyInt(schedulerName, "DD", 1);
	lastRun.wHour = option->GetPropertyInt(schedulerName, "H", 0);
	lastRun.wMinute = option->GetPropertyInt(schedulerName, "M", 0);
	lastRun.wSecond = option->GetPropertyInt(schedulerName, "S", 0);
	lastRun.wDayOfWeek = option->GetPropertyInt(schedulerName, "DW", 0);
	lastRun.wMilliseconds = option->GetPropertyInt(schedulerName, "MS", 0);

	if (lastRun.wYear > 1980)
		return SallyAPI::Date::DateHelper::GetDateString(lastRun, false);
	return "";
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SYSTEMTIME CSchedulerManager::GetLastSchedulerRun(const std::string& explicidAppName,
/// const std::string& identifier)
///
/// \brief	Gets the last scheduler run. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	explicidAppName	Name of the explicid application. 
/// \param	identifier		The identifier. 
///
/// \return	The last scheduler run. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SYSTEMTIME CSchedulerManager::GetLastSchedulerRun(const std::string& explicidAppName, const std::string& identifier)
{
	EnterCriticalSection(&m_critSect);

	std::vector<SallyAPI::Scheduler::CScheduler>::iterator iter = m_vScheduler.begin();
	while (iter != m_vScheduler.end())
	{
		SallyAPI::Scheduler::CScheduler& scheduler = *iter;

		SallyAPI::GUI::CAppBase* appBase = scheduler.GetReporterWindow();
		std::string ean = appBase->GetExplicitAppName();
		if ((ean.compare(explicidAppName) == 0) && (scheduler.GetIdentifier().compare(identifier) == 0))
		{
			SYSTEMTIME temp = GetLastSchedulerRun(scheduler);
			LeaveCriticalSection(&m_critSect);
			return temp;
		}
		++iter;
	}
	LeaveCriticalSection(&m_critSect);

	// Scheduler not found
	SYSTEMTIME noRun;

	noRun.wYear = 1980;
	noRun.wMonth = 1;
	noRun.wDay = 1;
	noRun.wHour = 0;
	noRun.wMinute = 0;
	noRun.wSecond = 0;
	noRun.wDayOfWeek = 0;
	noRun.wMilliseconds = 0;

	return noRun;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SYSTEMTIME CSchedulerManager::GetLastSchedulerRun(SallyAPI::GUI::CAppBase* appBase,
/// const std::string& identifier)
///
/// \brief	Gets the last scheduler run. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	appBase	If non-null, the application base. 
/// \param	identifier		The identifier. 
///
/// \return	The last scheduler run. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SYSTEMTIME CSchedulerManager::GetLastSchedulerRun(SallyAPI::GUI::CAppBase* appBase, const std::string& identifier)
{
	return GetLastSchedulerRun(appBase->GetExplicitAppName(), identifier);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SYSTEMTIME CSchedulerManager::GetLastSchedulerRun(SallyAPI::Scheduler::CScheduler& scheduler)
///
/// \brief	Gets the last scheduler run. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	scheduler	The scheduler. 
///
/// \return	The last scheduler run. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SYSTEMTIME CSchedulerManager::GetLastSchedulerRun(SallyAPI::Scheduler::CScheduler& scheduler)
{
	SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();

	SallyAPI::GUI::CAppBase* control = scheduler.GetReporterWindow();

	std::string schedulerName =  "scheduler#";
	schedulerName.append(scheduler.GetIdentifier());

	SallyAPI::Core::COptionManager* optionManager = SallyAPI::Core::COptionManager::GetInstance();
	SallyAPI::System::COption* option = optionManager->GetOption(control->GetExplicitAppName());

	// current Time
	SYSTEMTIME lastRun;

	lastRun.wYear = option->GetPropertyInt(schedulerName, "YYYY", 1980);
	lastRun.wMonth = option->GetPropertyInt(schedulerName, "MM", 1);
	lastRun.wDay = option->GetPropertyInt(schedulerName, "DD", 1);
	lastRun.wHour = option->GetPropertyInt(schedulerName, "H", 0);
	lastRun.wMinute = option->GetPropertyInt(schedulerName, "M", 0);
	lastRun.wSecond = option->GetPropertyInt(schedulerName, "S", 0);
	lastRun.wDayOfWeek = option->GetPropertyInt(schedulerName, "DW", 0);
	lastRun.wMilliseconds = option->GetPropertyInt(schedulerName, "MS", 0);

	return lastRun;
}