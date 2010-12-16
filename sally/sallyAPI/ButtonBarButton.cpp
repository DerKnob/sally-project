////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ButtonBarButton.cpp
///
/// \brief	Implements the button bar button class. 
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

#include "ButtonBarButton.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CButtonBarButton::CButtonBarButton(SallyAPI::GUI::CGUIBaseObject* parent, int width,
/// int controlId) :SallyAPI::GUI::CButton(parent, 0, 0, width, CONTROL_HEIGHT, controlId,
/// SallyAPI::GUI::BUTTON_TYPE_NORMAL), m_bFirst(false), m_bLast(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	width			The width. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CButtonBarButton::CButtonBarButton(SallyAPI::GUI::CGUIBaseObject* parent, int width, int controlId)
	:SallyAPI::GUI::CButton(parent, 0, 0, width, CONTROL_HEIGHT, controlId, SallyAPI::GUI::BUTTON_TYPE_NORMAL),
	m_bFirst(false), m_bLast(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CButtonBarButton::~CButtonBarButton()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CButtonBarButton::~CButtonBarButton()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButtonBarButton::SetFirst(bool first)
///
/// \brief	Sets a first. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	first	true to first. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButtonBarButton::SetFirst(bool first)
{
	m_bFirst = first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButtonBarButton::SetLast(bool last)
///
/// \brief	Sets a last. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	last	true to last. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButtonBarButton::SetLast(bool last)
{
	m_bLast = last;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CButtonBarButton::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CButtonBarButton::RenderControl()
{
	int borderLeft = 4;
	int borderRight = 4;

	int leftButton = -1;
	int rightButton = -1;

	if (!m_bEnabled)
	{
		leftButton = GUI_THEME_BUTTONBAR_SEPERATOR_DISABLED_LEFT;
		rightButton = GUI_THEME_BUTTONBAR_SEPERATOR_DISABLED_RIGHT;

		if (m_bFirst)
			leftButton = GUI_THEME_BUTTONBAR_DISABLED_LEFT;
		if (m_bLast)
			rightButton = GUI_THEME_BUTTONBAR_DISABLED_RIGHT;
		DrawButtonBackground(leftButton, GUI_THEME_BUTTONBAR_DISABLED, rightButton);
	}
	else if ((m_bChecked) || (m_bPressed))
	{
		leftButton = GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_LEFT;
		rightButton = GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_RIGHT;

		if (m_bFirst)
			leftButton = GUI_THEME_BUTTONBAR_SELECTED_LEFT;
		if (m_bLast)
			rightButton = GUI_THEME_BUTTONBAR_SELECTED_RIGHT;
		DrawButtonBackground(leftButton, GUI_THEME_BUTTONBAR_SELECTED, rightButton);
	}
	else
	{
		leftButton = GUI_THEME_BUTTONBAR_SEPERATOR_NORMAL_LEFT;
		rightButton = GUI_THEME_BUTTONBAR_SEPERATOR_NORMAL_RIGHT;

		if (m_bFirst)
			leftButton = GUI_THEME_BUTTONBAR_NORMAL_LEFT;
		if (m_bLast)
			rightButton = GUI_THEME_BUTTONBAR_NORMAL_RIGHT;
		DrawButtonBackground(leftButton, GUI_THEME_BUTTONBAR_NORMAL, rightButton);
	}

	if (m_iImage)
	{
		if (m_bImageLeft)
		{
			borderLeft += 8 + m_iImageSize;
			int imageWidthLeft = 4;

			if (m_strText.length() > 0)
			{
				// Temp variables
				SallyAPI::GUI::CPicture* image = NULL;

				if (m_bFirst)
					image = GetPicture(GUI_THEME_BUTTONBAR_NORMAL_LEFT);
				else
					image = GetPicture(GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_LEFT);
				if (image != NULL) {
					imageWidthLeft = image->GetWidth() + 4;
				}
			}
			else
			{
				imageWidthLeft = (m_iWidth - m_iImageSize) / 2;
			}

			int oldAlphaBlending = m_iAlphaBlending;
			if (!m_bEnabled)
				m_iAlphaBlending = m_iAlphaBlending / 2;

			DrawImage(m_iImage, imageWidthLeft, (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
			m_iAlphaBlending = oldAlphaBlending;
		}
		else
		{
			borderRight += 4 + m_iImageSize;
			int imageWidthRight = 0;

			if (m_strText.length() > 0)
			{
				// Temp variables
				SallyAPI::GUI::CPicture* image = NULL;

				if (m_bLast)
					image = GetPicture(GUI_THEME_BUTTONBAR_NORMAL_RIGHT);
				else
					image = GetPicture(GUI_THEME_BUTTONBAR_SEPERATOR_SELECTED_RIGHT);
				if (image != NULL) {
					imageWidthRight = image->GetWidth() + 4;
				}
			}

			int oldAlphaBlending = m_iAlphaBlending;
			if (!m_bEnabled)
				m_iAlphaBlending = m_iAlphaBlending / 2;

			DrawImage(m_iImage, m_iWidth - (m_iImageSize + imageWidthRight), (m_iHeight - m_iImageSize) / 2, m_iImageSize, m_iImageSize);
			m_iAlphaBlending = oldAlphaBlending;
		}
	}
	if ((m_bChecked) || (m_bPressed))
		DrawText(leftButton, rightButton, borderLeft, borderRight, "buttonbar.active.font");
	else
		DrawText(leftButton, rightButton, borderLeft, borderRight, "buttonbar.font");
}