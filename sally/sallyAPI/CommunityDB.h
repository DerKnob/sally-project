////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\CommunityDB.h
///
/// \brief	Declares the community database class. 
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
	namespace Community
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CCommunityDB
		///
		/// \brief	Community database. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CCommunityDB
		{
		private:
			static SallyAPI::Community::CCommunityDB*	m_pObject;

			std::string m_strMediaFolder;
			std::string m_strDatabaseFile;

			void CheckDatabaseExists();
			void AddNewStatus();
			void CleanUpStatus();
			void UpdateFriend(const std::string& userId, const std::string& nickName, const std::string& avatar);
			void AddStatus(const std::string& statusId, const std::string& userId, const std::string& messageString, const std::string& explicidAppName, const std::string& appName, const std::string& action, const std::string& actionName, const std::string& createDate);
			void CleanUp();

			CCommunityDB();
			~CCommunityDB();
		public:
			static SallyAPI::Community::CCommunityDB*	GetInstance();
			static void	DeleteInstance();

			void UpdateStatus(SallyAPI::GUI::CGUIBaseObject* mainWindow);
			std::vector<SallyAPI::Community::CStatusMessage> GetLastMessages(int count);
		};
	}
}
