////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScrollForm.cpp
///
/// \brief	Implements the scroll Windows Form. 
///
/// \author	Christian Knobloch
/// \date	07.07.2010
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

#include "ScrollForm.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollForm::CScrollForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId),
/// m_iScrollHeight(-1), m_iScrollWidth(-1)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollForm::CScrollForm(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId), m_iScrollHeight(-1), m_iScrollWidth(-1)
{
	m_pFormScrollbarVertical = new SallyAPI::GUI::CScrollbar(this, m_iWidth - CONTROL_HEIGHT, 0, CONTROL_HEIGHT, m_iHeight,
		SallyAPI::GUI::SCROLLBAR_ALIGNMENT_VERTICAL);
	m_pFormScrollbarVertical->Visible(false);
	this->AddChild(m_pFormScrollbarVertical);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScrollForm::~CScrollForm()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CScrollForm::~CScrollForm()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollForm::Resize(int width, int height)
///
/// \brief	Resizes. 
///
/// \author	Christian Knobloch
/// \date	07.07.2011
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollForm::Resize(int width, int height)
{
	SallyAPI::GUI::CForm::Resize(width, height);

	// move the scrollbar
	m_pFormScrollbarVertical->Move(m_iWidth - CONTROL_HEIGHT, m_iHeight);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScrollForm::ResizeScrollArea(int width, int height)
///
/// \brief	Resize scroll area. Set to -1 to disable scrollbars again.
///
/// \author	Christian Knobloch
/// \date	07.07.2011
///
/// \param	width	The width. 
/// \param	height	The height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScrollForm::ResizeScrollArea(int width, int height)
{
	m_iScrollWidth = width;
	m_iScrollHeight = height;

	//if (m_iScrollWidth > width)
	//	m_pFormScrollbarHorizontal->Visible(true);
	//else
	//	m_pFormScrollbarVertical->Visible(false);	

	if (m_iScrollHeight > height)
		m_pFormScrollbarVertical->Visible(true);
	else
		m_pFormScrollbarVertical->Visible(false);	
}