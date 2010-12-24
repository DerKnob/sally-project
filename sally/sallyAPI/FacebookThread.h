////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FacebookThread.h
///
/// \brief	Declares the facebook thread class. 
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
#include <vector>
#include "Thread.h"
#include "NetworkHelper.h"
#include "SallyHelper.h"

namespace SallyAPI
{
	namespace Facebook
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CFacebookThread
		///
		/// \brief	Facebook thread. 
		///
		/// \author	Christian Knobloch
		/// \date	24.12.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CFacebookThread :
			public SallyAPI::System::CThread
		{
		private:
			CRITICAL_SECTION			m_critSectLockRequest;
			std::vector<std::string>	m_vRequestList;

			virtual void RunEx();
		public:
			CFacebookThread();
			virtual ~CFacebookThread();

			void Request(const std::string& server, int port, const std::string& request, const std::string& post = "");
		};
	}
}