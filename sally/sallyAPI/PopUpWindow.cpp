////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\PopUpWindow.cpp
///
/// \brief	Implements the pop up Windows Form. 
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

#include "PopUpWindow.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPopUpWindow::CPopUpWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId,
/// const std::string &explicidAppName) :SallyAPI::GUI::CAppBase(parent, graphicId,
/// explicidAppName), m_bBlendInFromBottom(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	explicidAppName	Name of the explicid application. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CPopUpWindow::CPopUpWindow(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string &explicidAppName)
	:SallyAPI::GUI::CAppBase(parent, graphicId, explicidAppName), m_bBlendInFromBottom(false)
{
	Visible(false);
	SetAlphaBlending(0);

	m_pBackground = new SallyAPI::GUI::CImageBox(this, 0, 0, MAX_WIDTH, MAX_HEIGHT);
	m_pBackground->SetImageId(GUI_THEME_SALLY_POPUP_NORMAL);
	this->AddChild(m_pBackground);

	m_pButtonClose = new SallyAPI::GUI::CButton(this, WINDOW_WIDTH - 68, 28, 48 , 48, GUI_THEME_SALLY_CLOSE,
		SallyAPI::GUI::BUTTON_TYPE_ONLY_IMAGE);
	m_pButtonClose->SetImageId(GUI_THEME_SALLY_CLOSE);
	this->AddChild(m_pButtonClose);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CPopUpWindow::~CPopUpWindow()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CPopUpWindow::~CPopUpWindow()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPopUpWindow::Render()
///
/// \brief	Renders this object. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPopUpWindow::Render()
{
	int x = (WINDOW_WIDTH - MAX_WIDTH) / 2;
	int y = (WINDOW_HEIGHT - MAX_HEIGHT) / 2 - m_iYAbsolut;

	m_pBackground->Move(x, y);
	m_pButtonClose->Move(WINDOW_WIDTH - 60, 20 - m_iYAbsolut);
	SallyAPI::GUI::CAppBase::Render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPopUpWindow::BlendIn()
///
/// \brief	Blend in. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPopUpWindow::BlendIn()
{
	if (m_bBlendInFromBottom)
	{
		this->Move(0, WINDOW_HEIGHT);
	}
	else
	{
		this->Move(0, -WINDOW_HEIGHT);
	}

	this->BlendAnimated(255, 800, false);
	this->MoveAnimated(0, 0, 2500, false);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPopUpWindow::BlendOut()
///
/// \brief	Blend out. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPopUpWindow::BlendOut()
{
	this->BlendAnimated(0, 800, false);

	if (m_bBlendInFromBottom)
	{
		this->MoveAnimated(0, WINDOW_HEIGHT, 2500, false);
	}
	else
	{
		this->MoveAnimated(0, -WINDOW_HEIGHT, 2500, false);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPopUpWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
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

void CPopUpWindow::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_THEME_SALLY_CLOSE:
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_POPUP_VIEW);
			return;
		}
		break;
	}
	SallyAPI::GUI::CAppBase::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CPopUpWindow::BlendInFromBottom(bool value)
///
/// \brief	Blend in from bottom. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CPopUpWindow::BlendInFromBottom(bool value)
{
	m_bBlendInFromBottom = value;

	if (m_bBlendInFromBottom)
	{
		this->Move(0, WINDOW_HEIGHT);
	}
	else
	{
		this->Move(0, -WINDOW_HEIGHT);
	}
}