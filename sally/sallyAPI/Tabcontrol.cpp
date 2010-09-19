////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\Tabcontrol.cpp
///
/// \brief	Implements the tabcontrol class. 
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

#include "Tabcontrol.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTabcontrol::CTabcontrol(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width,
/// int height, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, height, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	height			The height. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CTabcontrol::CTabcontrol(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int height, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, height, controlId)
{
	m_eTabPosition = TAB_POSITION_LEFT;
	m_iXPosition = 0;
	m_iYPosition = 20;
	m_iLeftSpace = 150;
	m_pCurrentItem = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CTabcontrol::~CTabcontrol()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CTabcontrol::~CTabcontrol()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::TabPosition CTabcontrol::GetTabPosition()
///
/// \brief	Gets the tab position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The tab position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::TabPosition CTabcontrol::GetTabPosition()
{
	return m_eTabPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTabcontrol::SetTabPosition(SallyAPI::GUI::TabPosition tabPosition)
///
/// \brief	Sets a tab position. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	tabPosition	The tab position. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTabcontrol::SetTabPosition(SallyAPI::GUI::TabPosition tabPosition)
{
	m_eTabPosition = tabPosition;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTabcontrol::GetFormWidth()
///
/// \brief	Gets the form width. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The form width. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTabcontrol::GetFormWidth()
{
	return this->GetWidth() - m_iLeftSpace;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CTabcontrol::GetFormHeight()
///
/// \brief	Gets the form height. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The form height. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CTabcontrol::GetFormHeight()
{
	return this->GetHeight();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTabcontrol::AddTabItem(SallyAPI::GUI::CTabcontrolItem* item)
///
/// \brief	Adds a tab item. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	item	If non-null, the item. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTabcontrol::AddTabItem(SallyAPI::GUI::CTabcontrolItem* item)
{
	m_pTabcontrolItems.push_back(item);

	SallyAPI::GUI::CForm* form = item->GetForm();
	SallyAPI::GUI::CTabcontrolButton* button = item->GetButton();

	if (m_pCurrentItem == NULL)
	{
		m_pCurrentItem = item;
		button->SetActive(true);
		form->Visible(true);
	}
	this->AddChild(button);
	this->AddChild(form);

	button->Move(m_iXPosition, m_iYPosition);
	button->Resize(m_iLeftSpace, CONTROL_HEIGHT);

	int width = this->GetFormWidth();
	int height = this->GetFormHeight();

	form->Move(m_iLeftSpace, 0);
	form->Resize(width, height);

	m_iYPosition += 40;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTabcontrol::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTabcontrol::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::CTabcontrolButton* button = dynamic_cast<SallyAPI::GUI::CTabcontrolButton*> (reporter);
	if (button != NULL)
	{
		std::vector<SallyAPI::GUI::CTabcontrolItem*>::iterator iter = m_pTabcontrolItems.begin();

		while (iter != m_pTabcontrolItems.end())
		{
			SallyAPI::GUI::CTabcontrolItem* item = (*iter);

			if (item->GetButton() == button)
			{
				// deactive old
				m_pCurrentItem->GetButton()->SetActive(false);
				m_pCurrentItem->GetForm()->Visible(false);

				// active new
				item->GetButton()->SetActive(true);
				item->GetForm()->Visible(true);

				m_pCurrentItem = item;
				return;
			}
			++iter;
		}
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CTabcontrol::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CTabcontrol::RenderControl()
{
	SallyAPI::GUI::CPicture* image = NULL;

	int imageWidthLeftTop = 0;
	int imageWidthRightTop = 0;
	int imageWidthTop = 1;
	int imageHeightTop = 0;

	int imageWidthLeft = 0;
	int imageWidthCenter = 0;
	int imageWidthRight = 1;
	int imageHeightCenter = 0;

	int imageWidthLeftBottom = 0;
	int imageWidthRightBottom = 0;
	int imageWidthBottom = 1;
	int imageHeightBottom = 0;

	// Top
	image = GetPicture(GUI_THEME_TABCONTROL_LEFT_TOP);
	if (image != NULL) {
		imageWidthLeftTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_TABCONTROL_RIGHT_TOP);
	if (image != NULL) {
		imageWidthRightTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_TABCONTROL_TOP);
	if (image != NULL) {
		imageWidthTop = image->GetWidth();
		if (imageHeightTop == 0) {
			imageHeightTop = image->GetHeight();
		}
	}

	// Center
	image = GetPicture(GUI_THEME_TABCONTROL_LEFT);
	if (image != NULL) {
		imageWidthLeft = image->GetWidth();
	}
	image = GetPicture(GUI_THEME_TABCONTROL_RIGHT);
	if (image != NULL) {
		imageWidthRight = image->GetWidth();
	}
	image = GetPicture(GUI_THEME_TABCONTROL_CENTER);
	if (image != NULL) {
		imageWidthCenter = image->GetWidth();
	}

	// Bottom
	image = GetPicture(GUI_THEME_TABCONTROL_LEFT_BOTTOM);
	if (image != NULL) {
		imageWidthLeftBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_TABCONTROL_RIGHT_BOTTOM);
	if (image != NULL) {
		imageWidthRightBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}
	image = GetPicture(GUI_THEME_TABCONTROL_BOTTOM);
	if (image != NULL) {
		imageWidthBottom = image->GetWidth();
		if (imageHeightBottom == 0) {
			imageHeightBottom = image->GetHeight();
		}
	}

	imageHeightCenter = m_iHeight - (imageHeightBottom + imageHeightTop);

	// Top
	DrawImage(GUI_THEME_TABCONTROL_LEFT_TOP, m_iLeftSpace, 0);
	DrawImage(GUI_THEME_TABCONTROL_TOP, m_iLeftSpace + imageWidthLeftTop, 0, m_iWidth - (imageWidthLeftTop + imageWidthRightTop + m_iLeftSpace), imageHeightTop);
	DrawImage(GUI_THEME_TABCONTROL_RIGHT_TOP, m_iWidth - imageWidthRightTop, 0);

	// Center
	DrawImage(GUI_THEME_TABCONTROL_LEFT, m_iLeftSpace, imageHeightTop, imageWidthLeft, imageHeightCenter);
	DrawImage(GUI_THEME_TABCONTROL_CENTER, imageWidthLeft + m_iLeftSpace, imageHeightTop, m_iWidth - (imageWidthLeftBottom + imageWidthRightBottom +  m_iLeftSpace), imageHeightCenter);
	DrawImage(GUI_THEME_TABCONTROL_RIGHT, m_iWidth - imageWidthRight, imageHeightTop, imageWidthRight, imageHeightCenter);

	// Bottom
	DrawImage(GUI_THEME_TABCONTROL_LEFT_BOTTOM, m_iLeftSpace, imageHeightCenter + imageHeightTop);
	DrawImage(GUI_THEME_TABCONTROL_BOTTOM, imageWidthLeftBottom + m_iLeftSpace, imageHeightCenter + imageHeightTop, m_iWidth - (imageWidthLeftBottom + imageWidthRightBottom + m_iLeftSpace), imageHeightBottom);
	DrawImage(GUI_THEME_TABCONTROL_RIGHT_BOTTOM, m_iWidth - imageWidthRightBottom, imageHeightCenter + imageHeightTop);

	// Render Childs
	SallyAPI::GUI::CForm::RenderControl();
}