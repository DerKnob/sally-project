////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SmartThreadPool.cpp
///
/// \brief	Implements the smart thread pool class. 
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

#include "SmartThreadPool.h"

#include "SmartThreadPoolManager.h"

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartThreadPool::CSmartThreadPool()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartThreadPool::CSmartThreadPool()
{
	m_iConcurrentCount = 8;
	m_bAutoDelete = false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartThreadPool::~CSmartThreadPool()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartThreadPool::~CSmartThreadPool()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPool::SetConcurrentCount(int count)
///
/// \brief	Sets a concurrent count. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	count	Number of. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPool::SetConcurrentCount(int count)
{
	m_iConcurrentCount = count;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPool::SetAutoDelete(bool autoDelete)
///
/// \brief	Sets an automatic delete. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	autoDelete	true to automatic delete. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPool::SetAutoDelete(bool autoDelete)
{
	m_bAutoDelete = autoDelete;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPool::AddThread(SallyAPI::System::CThread* newThread)
///
/// \brief	Adds a thread. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	newThread	If non-null, the new thread. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPool::AddThread(SallyAPI::System::CThread* newThread)
{
	m_vThreadListNew.push_back(newThread);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPool::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPool::RunEx()
{
	while ((m_vThreadListNew.size() > 0) || (m_vThreadListRunning.size() > 0))
	{
		// start a new one
		while ((((int) m_vThreadListRunning.size()) < m_iConcurrentCount) &&
			(m_vThreadListNew.size() > 0))
		{
			StartNext();
		}

		// check for ended
		std::vector<SallyAPI::System::CThread*>::iterator iter = m_vThreadListRunning.begin();
		while (iter != m_vThreadListRunning.end())
		{
			SallyAPI::System::CThread* newThread = *iter;
			if (newThread->GetStatus() == SallyAPI::System::THREAD_STOPPED)
			{
				m_vThreadListRunning.erase(iter);

				SafeDelete(newThread);

				// reset the iterator
				iter = m_vThreadListRunning.begin();
			}
			else
			{
				++iter;
			}
		}

		Sleep(10);
	}

	if (m_bAutoDelete)
	{
		SallyAPI::System::CSmartThreadPoolManager* manager = SallyAPI::System::CSmartThreadPoolManager::GetInstance();
		manager->DeleteSmartThreadPool(this);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPool::StartNext()
///
/// \brief	Starts a next. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPool::StartNext()
{
	std::vector<SallyAPI::System::CThread*>::iterator iter = m_vThreadListNew.begin();
	if (iter != m_vThreadListNew.end())
	{
		SallyAPI::System::CThread* newThread = *iter;
		newThread->Start();

		m_vThreadListNew.erase(iter);
		m_vThreadListRunning.push_back(newThread);
	}
}