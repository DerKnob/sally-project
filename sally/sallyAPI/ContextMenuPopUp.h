////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ContextMenuPopUp.h
///
/// \brief	Declares the context menu pop up class. 
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
#include "PopUpWindow.h"
#include "ContextMenu.h"

namespace SallyAPI
{
	namespace GUI
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////
		/// \class	CContextMenuPopUp
		///
		/// \brief	Context menu pop up. 
		///
		/// \author	Christian Knobloch
		/// \date	30.03.2011
		////////////////////////////////////////////////////////////////////////////////////////////////////

		class DLL_API_SALLY CContextMenuPopUp :
			public SallyAPI::GUI::CPopUpWindow
		{
		protected:
			SallyAPI::GUI::CContextMenu*	m_pContextMenu;
		public:
			CContextMenuPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
				const std::string &explicidAppName, int width, int height);
			virtual ~CContextMenuPopUp();
		};
	}
}
