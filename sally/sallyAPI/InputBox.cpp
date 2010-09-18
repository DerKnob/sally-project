////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\InputBox.cpp
///
/// \brief	Implements the input box class. 
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

#include "InputBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CInputBox::CInputBox(SallyAPI::GUI::CGUIBaseObject* parent) :SallyAPI::GUI::CDialogBox(parent)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CInputBox::CInputBox(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CDialogBox(parent)
{
	m_pLableDialogText = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 20, 20, 500, 45);
	m_pLableDialogText->SetLocalised(true);
	m_pGroupBox->AddChild(m_pLableDialogText);

	m_pInput = new SallyAPI::GUI::CEdit(m_pGroupBox, 20, 20 + 45 + 10, 500);
	m_pInput->SetInfoText("click here to enter text");
	m_pGroupBox->AddChild(m_pInput);

	m_pButtonCancel = new SallyAPI::GUI::CButton(m_pGroupBox, 20, 160 - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, BUTTON_CANCEL, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonCancel->SetText("Cancel");
	m_pButtonCancel->SetAlign(DT_VCENTER | DT_CENTER);
	m_pGroupBox->AddChild(m_pButtonCancel);

	m_pButtonOk = new SallyAPI::GUI::CButton(m_pGroupBox, 540 - 150 - 20,  160 - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, BUTTON_OK, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonOk->SetText("Ok");
	m_pButtonOk->SetAlign(DT_VCENTER | DT_CENTER);
	m_pButtonOk->SetDefaultButton(true);
	m_pGroupBox->AddChild(m_pButtonOk);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CInputBox::~CInputBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CInputBox::~CInputBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CInputBox::SetValues(SallyAPI::GUI::SendMessage::CParameterInputBox& inputBox)
///
/// \brief	Sets the values. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	inputBox	The input box. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CInputBox::SetValues(SallyAPI::GUI::SendMessage::CParameterInputBox& inputBox)
{
	m_iControlId = inputBox.GetControlID();
	m_pNotificationWindow = inputBox.GetNotificationWindow();
	m_pLableDialogText->SetText(inputBox.GetDialogText());
	m_pInput->SetText(inputBox.GetDefaultText());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CInputBox::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
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

void CInputBox::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case BUTTON_OK:
			OnCommandButtonOk();
			return;
		case BUTTON_CANCEL:
			m_pNotificationWindow->SendMessageToParent(this, m_iControlId, MS_DIALOG_CANCEL);
			m_pParent->SendMessageToParent(this, m_iControlId, MS_DIALOG_CANCEL);
			return;
		}
		break;
	}
	SallyAPI::GUI::CDialog::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CInputBox::OnCommandButtonOk()
///
/// \brief	Executes the command button ok action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CInputBox::OnCommandButtonOk()
{
	SallyAPI::GUI::SendMessage::CParameterString messageString(m_pInput->GetText());
	m_pNotificationWindow->SendMessageToParent(this, m_iControlId, MS_DIALOG_OK, &messageString);
	m_pParent->SendMessageToParent(this, m_iControlId, MS_DIALOG_OK, &messageString);

}