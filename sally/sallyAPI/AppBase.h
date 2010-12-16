////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\AppBase.h
///
/// \brief	Declares the application base class. 
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
#include "OptionManager.h"
#include "Form.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CAppBase
		///
		/// \brief	Application base. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CAppBase :
			public SallyAPI::GUI::CForm
		{
		protected:
			int			m_iGraphicId;
			std::string	m_strExplicitAppName;
		public:
			CAppBase(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName);
			virtual ~CAppBase();

			// Helper Functions
			int			GetGraphicId();
			std::string GetExplicitAppName();

			std::string GetPropertyString(const std::string& ID, const std::string& defaultReturn = "");
			int			GetPropertyInt(const std::string& ID, int defaultReturn = -1);
			bool		GetPropertyBool(const std::string& ID, bool defaultReturn = false);
			void		SetPropertyString(const std::string& ID, const std::string& s);
			void		SetPropertyInt(const std::string& ID, int i);
			void		SetPropertyBool(const std::string& ID, bool i);
			int			GetPictureID(int iPicture);

			virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}