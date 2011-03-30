////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ContextMenuPopUp.cpp
///
/// \brief	Implements the context menu pop up class. 
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

#include "ContextMenuPopUp.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CContextMenuPopUp::CContextMenuPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
/// const std::string &explicidAppName) :SallyAPI::GUI::CPopUpWindow(parent, graphicId,
/// explicidAppName)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	explicidAppName	Name of the explicid application. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CContextMenuPopUp::CContextMenuPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
									 const std::string &explicidAppName, int width, int height)
	:SallyAPI::GUI::CPopUpWindow(parent, graphicId, explicidAppName)
{
	m_pContextMenu = new SallyAPI::GUI::CContextMenu(this, 0, 0, width, height);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CContextMenuPopUp::~CContextMenuPopUp()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	30.03.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CContextMenuPopUp::~CContextMenuPopUp()
{
}
