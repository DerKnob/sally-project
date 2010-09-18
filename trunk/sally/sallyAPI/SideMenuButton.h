////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\SideMenuButton.h
///
/// \brief	Declares the side menu button class. 
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
#include "Button.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \enum	SideMenuButtonType
		///
		/// \brief	Values that represent SideMenuButtonType. 
		////////////////////////////////////////////////////////////////////////////////////////////////////

		enum SideMenuButtonType {SIDE_MENUE_BUTTON_TYPE_NORMAL, SIDE_MENUE_BUTTON_TYPE_SEPERATOR, SIDE_MENUE_BUTTON_TYPE_HEADING};

		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CSideMenuButton
		///
		/// \brief	Side menu button. 
		///
		/// \author	Christian Knobloch
		/// \date	19.04.2010
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CSideMenuButton :
			public SallyAPI::GUI::CButton
		{
		protected:
			SallyAPI::GUI::SideMenuButtonType	m_eSideMenuType;

			virtual void	RenderControl();
		public:
			CSideMenuButton(SallyAPI::GUI::CGUIBaseObject* parent, SallyAPI::GUI::SideMenuButtonType sideMenuType,int controlId = 0);
			virtual ~CSideMenuButton();
		};
	}
}