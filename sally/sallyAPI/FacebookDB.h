////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FacebookDB.h
///
/// \brief	Declares the facebook database class. 
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
#include <tchar.h>
#include "Define.h"
#include "GUIBaseObject.h"
#include "StatusMessage.h"

namespace SallyAPI
{
	namespace Facebook
	{

		class DLL_API_SALLY CFacebookDB
		{
		private:
			static SallyAPI::Facebook::CFacebookDB*	m_pObject;

			std::string m_strMediaFolder;
			std::string m_strDatabaseFile;

			void CheckDatabaseExists();
			void AddNewStatus();
			void CleanUpStatus();
			void UpdateFriend(const std::string& userId, const std::string& name);
			void AddStatus(const std::string& statusId, const std::string& userId, const std::string& messageString, const std::string& explicidAppName, const std::string& appName, const std::string& action, const std::string& actionName, const std::string& createDate);
			void CleanUp();

			CFacebookDB();
			~CFacebookDB();
		public:
			static SallyAPI::Facebook::CFacebookDB*	GetInstance();
			static void	DeleteInstance();

			std::vector<std::string>	GetFriendIds();
			void						UpdateUserInfo();
			bool						GetStatusMessages(SallyAPI::GUI::CGUIBaseObject* mainWindow);
			std::vector<SallyAPI::Facebook::CStatusMessage> GetLastMessages(int count);
		};
	}
}
