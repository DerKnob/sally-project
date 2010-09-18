////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SmartThreadPoolManager.cpp
///
/// \brief	Implements the smart thread pool manager class. 
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

#include "SmartThreadPoolManager.h"

SallyAPI::System::CSmartThreadPoolManager*	SallyAPI::System::CSmartThreadPoolManager::m_pObject = NULL;

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartThreadPoolManager::CSmartThreadPoolManager()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartThreadPoolManager::CSmartThreadPoolManager()
{
	m_tThreadPoolMangerThread = new SallyAPI::System::CSmartThreadPollManagerThread();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartThreadPoolManager::~CSmartThreadPoolManager()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartThreadPoolManager::~CSmartThreadPoolManager()
{
	m_tThreadPoolMangerThread->WaitForStop();
	SafeDelete(m_tThreadPoolMangerThread);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPoolManager::DeleteInstance()
///
/// \brief	Deletes an instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPoolManager::DeleteInstance()
{
	SafeDelete(m_pObject);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::System::CSmartThreadPoolManager* CSmartThreadPoolManager::GetInstance()
///
/// \brief	Gets the instance. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	null if it fails, else the instance. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::System::CSmartThreadPoolManager* CSmartThreadPoolManager::GetInstance()
{
	if (m_pObject == NULL) {
		m_pObject = new SallyAPI::System::CSmartThreadPoolManager();
	}
	return m_pObject;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPoolManager::DeleteSmartThreadPool(SallyAPI::System::CSmartThreadPool* smartThreadPool)
///
/// \brief	Deletes the smart thread pool described by smartThreadPool. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	smartThreadPool	If non-null, the smart thread pool. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPoolManager::DeleteSmartThreadPool(SallyAPI::System::CSmartThreadPool* smartThreadPool)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	m_vThreadList.push_back(smartThreadPool);

	m_tThreadPoolMangerThread->Start();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CSmartThreadPoolManager::DeleteNext()
///
/// \brief	Deletes a next. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	true if it succeeds, false if it fails. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CSmartThreadPoolManager::DeleteNext()
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::System::CSmartThreadPool*>::iterator iter = m_vThreadList.begin();
	if (iter != m_vThreadList.end())
	{
		SallyAPI::System::CSmartThreadPool* newThreadPool = *iter;

		if (newThreadPool->GetStatus() == SallyAPI::System::THREAD_STOPPED)
		{
			m_vThreadList.erase(iter);
			SafeDelete(newThreadPool);
		}
	}

	return (m_vThreadList.size() > 0);
}