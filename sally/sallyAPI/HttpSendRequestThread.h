////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\HttpSendRequestThread.h
///
/// \brief	Declares the http send request thread class. 
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
#include "Thread.h"
#include <Wininet.h>
#include <string>

namespace SallyAPI
{
	namespace Network
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CHttpSendRequestThread
		///
		/// \brief	Http send request thread. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CHttpSendRequestThread
				: public SallyAPI::System::CThread
		{
		private:
			HINTERNET	m_pHttpRequest;
			std::string m_strPostData;
			int			m_iErrorCode;

			virtual void RunEx();
		public:
			CHttpSendRequestThread();
			virtual ~CHttpSendRequestThread();

			void	SetValues(HINTERNET httpRequest);
			void	SetValues(HINTERNET httpRequest, const std::string& postData);

			int		GetErrorCode();
		};
	}
}