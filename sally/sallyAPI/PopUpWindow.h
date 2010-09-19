////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\PopUpWindow.h
///
/// \brief	Declares the pop up Windows Form. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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
#include "AppBase.h"
#include "Button.h"
#include "ImageBox.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CPopUpWindow
		///
		/// \brief	Form for viewing the c pop up. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CPopUpWindow :
			public SallyAPI::GUI::CAppBase
		{
		protected:
			bool						m_bBlendOut;
			bool						m_bBlendInFromBottom;
			SallyAPI::GUI::CButton*		m_pButtonClose;
			SallyAPI::GUI::CImageBox*	m_pBackground;
		public:
			CPopUpWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string &explicidAppName);
			virtual ~CPopUpWindow();

			void			BlendInFromBottom(bool value);

			virtual void	BlendIn();
			virtual void	BlendOut();

			virtual void Render();
			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}