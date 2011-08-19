////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\FacebookManager.h
///
/// \brief	Declares the facebook manager class. 
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
#include "FacebookThread.h"

namespace SallyAPI
{
	namespace Facebook
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CFacebookManager
		///
		/// \brief	Manager for facebooks. 
		///
		/// \author	Christian Knobloch
		/// \date	24.12.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CFacebookManager
		{
		private:
			static SallyAPI::Facebook::CFacebookManager*		m_pObject;
			SallyAPI::Facebook::CFacebookThread					m_tFacebookThread;
			std::vector<SallyAPI::GUI::CGUIBaseObject*>			m_pNotifierWindows;
			std::map<std::string, int>							m_mUserImageId;
			int													m_iUserImageCounter;
			SallyAPI::System::CCritSection						m_UserImagesLock;


			std::string	GenerateBaseRequest(const std::string& menu);
			void		GenerateSallyKey();
			bool		LoadFacebookUserImage(const std::string& userId);

			CFacebookManager();
			~CFacebookManager();
		public:
			static SallyAPI::Facebook::CFacebookManager*	GetInstance();
			static void	DeleteInstance();

			bool		IsEnabled();

			std::string RequestData(const std::string& action, std::map<std::string, std::string>& requestMap,
				std::string& errorMessage, SallyAPI::Network::NETWORK_RETURN& errorCode);
			bool		SendData(const std::string& action, std::map<std::string, std::string>& requestMap);
			bool		SendStatusMessage(const std::string& explicidAppName, const std::string& appName,
				const std::string& message, const std::string& action, const std::string& actionName);

			// attach listeners
			void		SendUpdateToRegistedNotifier();
			void		RegisterStatusUpdateNotifier(SallyAPI::GUI::CGUIBaseObject* window);

			bool		UpdateFacebookUserInfo();

			std::string GetFacebookUserName();
			std::string GetFacebookUserId();
			std::string GetSallyKey();
			std::string GetFacebookAccessToken();

			void		SetFacebookUserName(const std::string& userName);

			void		Disable();
			void		Enable(const std::string& accessToken, const std::string& userName, 
				const std::string& userId);

			bool		ActivateFacebook();
			void		ConnectFacebook();
			void		ShowErrorMessage(SallyAPI::GUI::CGUIBaseObject* mainWindow);

			bool		PostMessageToWall(const std::string& message, const std::string& description, 
				const std::string& link, const std::string& image, 
				std::string& errorMessage);
			bool		GetWall(std::string& dataResponse, std::string& errorMessage,
				SallyAPI::Network::NETWORK_RETURN& errorCode);
			bool		GetNews(std::string& dataResponse, std::string& errorMessage,
				SallyAPI::Network::NETWORK_RETURN& errorCode);

			// Image management
			int			GetFacebookUserImageId(const std::string& userId);
			void		DownloadFacebookUserImage(const std::string& imageFolder, const std::string& userId);
			bool		ReloadAllFacebookUserImages();
		};
	}
}
