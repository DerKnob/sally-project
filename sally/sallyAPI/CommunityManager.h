////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\CommunityManager.h
///
/// \brief	Declares the community manager class. 
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
#include <string>
#include "NetworkHelper.h"
#include "Scheduler.h"
#include "CommunityThread.h"

namespace SallyAPI
{
	namespace Community
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CCommunityManager
		///
		/// \brief	Manager for communities. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CCommunityManager
		{
		private:
			static SallyAPI::Community::CCommunityManager*	m_pObject;
			bool											m_bLastRequestSuccess;
			std::string										m_strErrorMessage;
			CCommunityThread								m_tCommunityThread;
			std::vector<SallyAPI::GUI::CGUIBaseObject*>		m_pNotifierWindows;

			std::string	GenerateBaseRequest(const std::string& menu);

			CCommunityManager();
			~CCommunityManager();
		public:
			static SallyAPI::Community::CCommunityManager*	GetInstance();
			static void	DeleteInstance();

			bool		IsEnabled();
			bool		TestLogin(std::string& errorMessage);
			std::string RequestData(const std::string& action, std::map<std::string, std::string>& requestMap,
				std::string& errorMessage);
			bool		SendData(const std::string& action, std::map<std::string, std::string>& requestMap);
			bool		SendStatusMessage(const std::string& explicidAppName, const std::string& appName,
				const std::string& message, const std::string& action, const std::string& actionName);
			std::string	GetUsername();
			std::string	GetPassword();

			void		SendUpdateToRegistedNotifier();
			void		RegisterStatusUpdateNotifier(SallyAPI::GUI::CGUIBaseObject* window);

			std::string	GetLastRequestErrorMessage();
			bool		LastRequestSuccess();
		};
	}
}
