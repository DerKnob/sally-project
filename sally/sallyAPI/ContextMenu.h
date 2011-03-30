////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ContextMenu.h
///
/// \brief	Declares the context menu class. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
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
#include "Form.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	CONTEXT_MENU_ARROW_POSITION
		///
		/// \brief	Values that represent CONTEXT_MENU_ARROW_POSITION. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum CONTEXT_MENU_ARROW_POSITION {CONTEXT_MENU_ARROW_POSITION_LEFT_TOP, CONTEXT_MENU_ARROW_POSITION_LEFT_BOTTOM,
			CONTEXT_MENU_ARROW_POSITION_RIGHT_TOP, CONTEXT_MENU_ARROW_POSITION_RIGHT_BOTTOM,
			CONTEXT_MENU_ARROW_POSITION_CENTER_TOP, CONTEXT_MENU_ARROW_POSITION_CENTER_BOTTOM};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CContextMenu
		///
		/// \brief	Pop up menu. 
		///
		/// \author	Christian Knobloch
		/// \date	30.03.2011
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CContextMenu :
			public SallyAPI::GUI::CForm
		{
		protected:
			CONTEXT_MENU_ARROW_POSITION		m_ePosition;
			virtual void					RenderControl();
		public:
			CContextMenu(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId = 0);
			virtual ~CContextMenu();

			void SetArrowPosition(CONTEXT_MENU_ARROW_POSITION position);
		};
	}
}