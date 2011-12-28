////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScreensaverControl.h
///
/// \brief	Declares the screensaver controls class. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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
#include <vector>
#include "Form.h"
#include "ScreensaverControlButton.h"
#include "ApplicationWindow.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CScreensaverControl
		///
		/// \brief	Screensaver control. 
		///
		/// \author	Christian Knobloch
		/// \date	27.12.2011
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CScreensaverControl
			: public SallyAPI::GUI::CForm
		{
		private:
			SallyAPI::GUI::CApplicationWindow*		m_pApp;
			std::vector<SallyAPI::GUI::CControl*>	m_vControl;
			bool									m_bShowAlways;

			virtual void	RenderControl();
		public:
			CScreensaverControl(SallyAPI::GUI::CGUIBaseObject* parent, SallyAPI::GUI::CApplicationWindow* app);
			virtual ~CScreensaverControl();

			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void AddChild(SallyAPI::GUI::CControl* control);
			
			void	ShowAlways(bool value);
			bool	IsShowAlways();

			SallyAPI::GUI::CApplicationWindow*		GetApplicationWindow();
		};
	}
}