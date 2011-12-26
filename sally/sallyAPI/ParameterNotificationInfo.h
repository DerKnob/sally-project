////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ParameterNotificationInfo.h
///
/// \brief	Declares the parameter information popup class. 
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
#include "ParameterBase.h"
#include "Picture.h"
#include "ApplicationWindow.h"

namespace SallyAPI
{
	namespace GUI
	{
		namespace SendMessage
		{
			////////////////////////////////////////////////////////////////////////////////////////////////////
			/// \class	CParameterNotificationInfo
			///
			/// \brief	Information about the parameter notification. 
			///
			/// \author	Christian Knobloch
			/// \date	26.12.2011
			////////////////////////////////////////////////////////////////////////////////////////////////////

			class DLL_API_SALLY CParameterNotificationInfo
				: public SallyAPI::GUI::SendMessage::CParameterBase
			{
			private:
				int									m_iIcon;
				std::string							m_strText;
				std::string							m_strDescription;
				SallyAPI::GUI::CPicture*			m_pPicture;
				int									m_pID;
			public:
				CParameterNotificationInfo(int icon, const std::string& text = "", const std::string& description = "");
				CParameterNotificationInfo(SallyAPI::GUI::CPicture* picture, const std::string& text = "", const std::string& description = "");
				virtual ~CParameterNotificationInfo();

				std::string GetDescription();
				std::string GetText();
				int			GetIcon();
				SallyAPI::GUI::CPicture*			GetPicture();

				int			GetId();
				void		SetId(int id);
			};
		}
	}
}