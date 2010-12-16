////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SmartThreadPoolManager.h
///
/// \brief	Declares the smart thread pool manager class. 
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
#include "SmartThreadPool.h"
#include "AutoLock.h"
#include "CritSection.h"
#include "SmartThreadPollManagerThread.h"

namespace SallyAPI
{
	namespace System
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CSmartThreadPoolManager
		///
		/// \brief	Manager for smart thread pools. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CSmartThreadPoolManager
		{
		private:
			static SallyAPI::System::CSmartThreadPoolManager*	m_pObject;

			SallyAPI::System::CSmartThreadPollManagerThread*	m_tThreadPoolMangerThread;
			std::vector<SallyAPI::System::CSmartThreadPool*>	m_vThreadList;
			SallyAPI::System::CCritSection						m_Lock;
			
			CSmartThreadPoolManager();
			~CSmartThreadPoolManager();
		public:
			static SallyAPI::System::CSmartThreadPoolManager*	GetInstance();
			static void	DeleteInstance();

			void DeleteSmartThreadPool(SallyAPI::System::CSmartThreadPool* smartThreadPool);
			bool DeleteNext();
		};
	}
}
