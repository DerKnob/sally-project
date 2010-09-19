////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Tabcontrol.h
///
/// \brief	Declares the tabcontrol class. 
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
#include "Form.h"
#include "TabcontrolItem.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	TabPosition
		///
		/// \brief	Values that represent TabPosition. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum TabPosition {TAB_POSITION_LEFT};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CTabcontrol
		///
		/// \brief	Tabcontrol. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CTabcontrol
			: public SallyAPI::GUI::CForm
		{
		private:
			std::vector<SallyAPI::GUI::CTabcontrolItem*>	m_pTabcontrolItems;
			SallyAPI::GUI::CTabcontrolItem*					m_pCurrentItem;
			TabPosition		m_eTabPosition;
			int				m_iXPosition;
			int				m_iYPosition;
			int				m_iLeftSpace;

			virtual void	RenderControl();
		public:
			CTabcontrol(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0);
			virtual ~CTabcontrol();

			void						SetTabPosition(SallyAPI::GUI::TabPosition tabPosition);
			SallyAPI::GUI::TabPosition	GetTabPosition();

			void	AddTabItem(SallyAPI::GUI::CTabcontrolItem* item);
			int		GetFormWidth();
			int		GetFormHeight();

			virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
		};
	}
}