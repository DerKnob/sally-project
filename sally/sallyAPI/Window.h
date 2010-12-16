////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Window.h
///
/// \brief	Declares the window class. 
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
#include <list>
#include "GUIBaseObject.h"
#include "Control.h"
#include "WindowMessage.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CWindow
		///
		/// \brief	Form for viewing the c. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CWindow
			: public SallyAPI::GUI::CGUIBaseObject
		{
		protected:
			std::list<CControl*>		m_GUIControlList;
			std::list<CWindowMessage*>	m_iMessages;
			
			virtual	void	RenderControl();
			virtual	void	UpdateControl();
		public:
			CWindow();
			virtual ~CWindow();

			virtual void			ProcessKeyboard(char KeyboardState[256], char KeyboardStatePressed[256]);

			virtual bool			CharInputPressed(char c);
			virtual bool			KeyDown(int c);
			virtual bool			ProcessMouseDoubleClick(int x, int y);
			virtual bool			ProcessMouseUp(int x, int y);
			virtual bool			ProcessMouseDown(int x, int y);
			virtual bool			ProcessMouseMove(int x, int y);

			virtual void			SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void			SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual CWindowMessage	GetMessage();
			virtual void			Timer(float timeDelta);

			virtual void	GetAbsolutPosition(int* x, int* y);

			virtual void	Render();
			virtual void	Update();
			virtual void	SaveConfig();

			void			AddChild(SallyAPI::GUI::CControl* control);
			void			RemoveChild(std::list<SallyAPI::GUI::CControl*>::iterator iter);
			void			RemoveChild(SallyAPI::GUI::CControl* control);
		};
	}
}