////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SmartThreadPollManagerThread.cpp
///
/// \brief	Implements the smart thread poll manager thread class. 
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

#include "SmartThreadPollManagerThread.h"

#include "SmartThreadPoolManager.h"

using namespace SallyAPI::System;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartThreadPollManagerThread::CSmartThreadPollManagerThread()
///
/// \brief	Default constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartThreadPollManagerThread::CSmartThreadPollManagerThread()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CSmartThreadPollManagerThread::~CSmartThreadPollManagerThread()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CSmartThreadPollManagerThread::~CSmartThreadPollManagerThread()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CSmartThreadPollManagerThread::RunEx()
///
/// \brief	Executes the ex operation. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CSmartThreadPollManagerThread::RunEx()
{
	SallyAPI::System::CSmartThreadPoolManager* manager = SallyAPI::System::CSmartThreadPoolManager::GetInstance();

	while (manager->DeleteNext())
	{
		Sleep(1);
	}
}