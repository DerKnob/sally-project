////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Scrollbar.h
///
/// \brief	Declares the scrollbar class. 
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
#include "Control.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	SCROLLBAR_ALIGNMENT
		///
		/// \brief	Values that represent SCROLLBAR_ALIGNMENT. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum SCROLLBAR_ALIGNMENT {SCROLLBAR_ALIGNMENT_VERTICAL }; //, SCROLLBAR_ALIGNMENT_HORIZONTAL};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CScrollbar
		///
		/// \brief	Scrollbar. 
		///
		/// \author	Christian Knobloch
		/// \date	30.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CScrollbar :
			public SallyAPI::GUI::CControl
		{
		private:
			int						m_iMouseDownKnobOffSet;

			SCROLLBAR_ALIGNMENT		m_eScrollbarAlignment;
			int						m_iPosition;
			int						m_iPositionMax;
			int						m_iKnobHeight;
			int						m_iKnobPosition;
			int						m_iBarHeight;
			bool					m_bShowScrollbarIfNotScrollable;

			void			DrawHorizontal();
			void			DrawVertical();

			virtual void	RenderControl();
			virtual bool	ProcessMouseDown(int x, int y);
			virtual bool	ProcessMouseUp(int x, int y);
			virtual bool	ProcessMouseMove(int x, int y);
		public:
			CScrollbar(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, SCROLLBAR_ALIGNMENT scrollbarAlignment, int controlId = 0);
			virtual ~CScrollbar();

			void		ShowScrollbarIfNotScrollable(bool value);
			bool		IsScrollbarVisibleIfNotScrollbable();

			void		SetMaxPosition(int position);
			void		SetPosition(int position);
			int			GetMaxPosition();
			int			GetPosition();

			virtual void SendMessageToParent(CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual void SendMessageToChilds(CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
			virtual bool CheckProcessMouseDown(int x, int y);
		};
	}
}