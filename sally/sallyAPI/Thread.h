////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Thread.h
///
/// \brief	Declares the thread class. 
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
#include "SystemHelper.h"
#include "AutoLock.h"
#include "CritSection.h"

namespace SallyAPI
{
	namespace System
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	ThreadStatus
		///
		/// \brief	Values that represent ThreadStatus. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum ThreadStatus {THREAD_STOPPED, THREAD_RUNNING};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CThread
		///
		/// \brief	Thread. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CThread
		{
		protected:
			ThreadStatus						m_eStatus;
			HANDLE								m_hThread;
			bool								m_bPleaseStop;
			SallyAPI::System::CCritSection		m_Lock;
			std::string							m_strId;

			static DWORD WINAPI	Thread(PVOID pvoid);
			virtual void	RunEx() = 0;
			void			Run();
		public:
			CThread();
			virtual ~CThread();

			ThreadStatus	GetStatus();
			virtual void	Start();
			virtual void	Stop(bool force = false);
			virtual void	WaitForStop(bool force = false);
			std::string		GetId();
		};
	}
}