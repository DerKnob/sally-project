////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\StatusMessage.h
///
/// \brief	Declares the status message class. 
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

namespace SallyAPI
{
	namespace Facebook
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CStatusMessage
		///
		/// \brief	Status message. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CStatusMessage
		{
		private:
			std::string m_strCreateDate;
			std::string m_strName;
			std::string m_strExplicidAppName;
			std::string m_strAppName;
			std::string m_strAction;
			std::string m_strMessageString;
			std::string m_strActionName;
			std::string m_strUserId;
		public:
			CStatusMessage(const std::string& createDate, const std::string& userId,
				const std::string& name, const std::string& explicidAppName,
				const std::string& appName, const std::string& action,
				const std::string& actionName, const std::string& messageString);
			~CStatusMessage();

			std::string GetUserId();
			std::string GetName();
			std::string GetExplicidAppName();
			std::string GetAppName();
			std::string GetAction();
			std::string GetActionName();
			std::string GetMessageString();
			std::string GetCreateDate();
		};
	}
}