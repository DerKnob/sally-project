////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Form.h
///
/// \brief	Declares the form class. 
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

#include "Control.h"
#include <list>
#include <set>

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	ScrollType
		///
		/// \brief	Values that represent ScrollType. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum ScrollType {SCROLL_TYPE_NONE, SCROLL_TYPE_NORMAL, SCROLL_TYPE_SMOOTH};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CForm
		///
		/// \brief	Form for viewing the c. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CForm :
			public SallyAPI::GUI::CControl
		{
		protected:
			std::list<SallyAPI::GUI::CControl*>	m_GUIControlList;
			
			float				m_fStartSendMessage;
			float				m_fStartMouseDown;
			int					m_iSetMessage;
			ScrollType			m_eScrollType;
			int					m_iScrollSmoothMouseOldX;
			int					m_iScrollSmoothMouseOldY;
			std::vector<int>	m_vScrollSmoothLastX;
			std::vector<int>	m_vScrollSmoothLastY;
			std::vector<float>	m_vScrollSmoothLastTime;
			bool				m_bHandleInputIfItIsChildHandled;
			bool				m_bAlwaysHandleInput;
			bool				m_bStopOnFirstSuccess;
			bool				m_bScrolling;

			bool			MoveX(int x);
			bool			MoveY(int y);
			virtual void	RenderControl();
			virtual void	UpdateControl();
			virtual void	TimerScrollNormal();
			virtual bool	ScrollTypeNormal(int x, int y);
			virtual bool	ScrollTypeSmooth(int x, int y);

			virtual bool	ProcessMouseDown(int x, int y);
			virtual bool	ProcessMouseUp(int x, int y);
			virtual bool	ProcessMouseMove(int x, int y);
			virtual bool	ProcessMouseDoubleClick(int x, int y);
		public:
			CForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0);
			virtual ~CForm();

			virtual void SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void SendMessageToParent(CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void Timer(float timeDelta);
			virtual void SetAlphaBlending(int alphaBlending);
			virtual void Render();
			virtual void Update();

			virtual void AddChild(SallyAPI::GUI::CControl* control);
			void	RemoveChild(std::list<CControl*>::iterator iter);

			void	SetScrollType(SallyAPI::GUI::ScrollType scrollType);
			void	SetHandleInputIfItIsChildHandled(bool handleInputIfItIsChildHandled);
			void	SetAlwaysHandleInput(bool alwaysHandleInput);
			void	SetStopHandleInputOnFirstSuccess(bool stopOnFirstSuccess);
			bool	IsScrolling();
		};
	}
}