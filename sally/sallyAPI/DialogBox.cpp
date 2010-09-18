////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\DialogBox.cpp
///
/// \brief	Implements the dialog box class. 
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

#include "DialogBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDialogBox::CDialogBox(SallyAPI::GUI::CGUIBaseObject* parent) :SallyAPI::GUI::CDialog(parent)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CDialogBox::CDialogBox(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CDialog(parent)
{
	m_pGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 540) / 2, (WINDOW_HEIGHT - 150) / 2, 540, 160);
	this->AddChild(m_pGroupBox);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CDialogBox::~CDialogBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CDialogBox::~CDialogBox()
{
}
