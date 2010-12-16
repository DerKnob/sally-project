////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\MessageBox.cpp
///
/// \brief	Implements the message box class. 
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

#include "MessageBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CMessageBox::CMessageBox(SallyAPI::GUI::CGUIBaseObject* parent) :SallyAPI::GUI::CDialogBox(parent)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CMessageBox::CMessageBox(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CDialogBox(parent)
{
	m_pImage = new SallyAPI::GUI::CImageBox(m_pGroupBox, -20, -20, 64, 64);
	m_pGroupBox->AddChild(m_pImage);

	m_pLableDialogText = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 55, 20, 540 - 20 - 55, 80);
	m_pLableDialogText->SetLocalised(true);
	m_pGroupBox->AddChild(m_pLableDialogText);

	m_pButtonOk = new SallyAPI::GUI::CButton(m_pGroupBox, 540 - 150 - 20,  160 - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, BUTTON_OK, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonOk->SetText("Ok");
	m_pButtonOk->SetAlign(DT_VCENTER | DT_CENTER);
	m_pButtonOk->SetDefaultButton(true);
	m_pGroupBox->AddChild(m_pButtonOk);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CMessageBox::~CMessageBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CMessageBox::~CMessageBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CMessageBox::SetValues(SallyAPI::GUI::SendMessage::CParameterMessageBox& messageBox)
///
/// \brief	Sets the values. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageBox	The message box. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CMessageBox::SetValues(SallyAPI::GUI::SendMessage::CParameterMessageBox& messageBox)
{
	m_iControlId = messageBox.GetControlID();
	m_pNotificationWindow = messageBox.GetNotificationWindow();
	m_pLableDialogText->SetText(messageBox.GetDialogText());
	switch (messageBox.GetIcon())
	{
	case MESSAGEBOX_ICON_ERROR:
		m_pImage->SetImageId(GUI_THEME_SALLY_ICON_MB_ERROR);
		break;
	case MESSAGEBOX_ICON_WARNING:
		m_pImage->SetImageId(GUI_THEME_SALLY_ICON_MB_WARNING);
		break;
	default:
		m_pImage->SetImageId(GUI_THEME_SALLY_ICON_MB_INFO);
		break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CMessageBox::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
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

void CMessageBox::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case BUTTON_OK:
			m_pNotificationWindow->SendMessageToParent(this, m_iControlId, MS_DIALOG_OK);
			m_pParent->SendMessageToParent(this, m_iControlId, MS_DIALOG_OK);
			return;
		}
		break;
	}
	SallyAPI::GUI::CDialog::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}