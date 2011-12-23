////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\QuestionBox.cpp
///
/// \brief	Implements the question box class. 
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

#include "QuestionBox.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CQuestionBox::CQuestionBox(SallyAPI::GUI::CGUIBaseObject* parent) :SallyAPI::GUI::CDialogBox(parent)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CQuestionBox::CQuestionBox(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CDialogBox(parent)
{
	m_pImage = new SallyAPI::GUI::CImageBox(m_pGroupBox, -20, -20, 64, 64);
	m_pImage->SetImageId(GUI_THEME_SALLY_ICON_MB_QUESTION);
	m_pGroupBox->AddChild(m_pImage);

	m_pLableDialogText = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 55, 20, 540 - 20 - 55, 80);
	m_pLableDialogText->SetLocalised(true);
	m_pLableDialogText->SetAutoResize(true);
	m_pGroupBox->AddChild(m_pLableDialogText);

	m_pButtonNo = new SallyAPI::GUI::CButton(m_pGroupBox, 20, 160 - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, BUTTON_NO, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonNo->SetText("No");
	m_pButtonNo->SetAlign(DT_VCENTER | DT_CENTER);
	m_pGroupBox->AddChild(m_pButtonNo);

	m_pButtonYes = new SallyAPI::GUI::CButton(m_pGroupBox, 540 - 150 - 20,  160 - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, BUTTON_YES, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonYes->SetText("Yes");
	m_pButtonYes->SetAlign(DT_VCENTER | DT_CENTER);
	m_pGroupBox->AddChild(m_pButtonYes);

}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CQuestionBox::~CQuestionBox()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CQuestionBox::~CQuestionBox()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CQuestionBox::SetValues(SallyAPI::GUI::SendMessage::CParameterQuestionBox& questionBox)
///
/// \brief	Sets the values. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	questionBox	The question box. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CQuestionBox::SetValues(SallyAPI::GUI::SendMessage::CParameterQuestionBox& questionBox)
{
	m_iControlId = questionBox.GetControlID();
	m_pNotificationWindow = questionBox.GetNotificationWindow();
	m_pLableDialogText->SetText(questionBox.GetDialogText());

	int width = m_pLableDialogText->GetWidth();
	int height = m_pLableDialogText->GetHeight();

	if (width < 300)
		width = 300;

	if (height < CONTROL_HEIGHT)
		height = CONTROL_HEIGHT;

	m_pGroupBox->Resize(width + 55 + 20, height + 20 + 20 + 20 + CONTROL_HEIGHT);
	m_pButtonYes->Move(m_pGroupBox->GetWidth() - 150 - 20, m_pGroupBox->GetHeight() - 20 - CONTROL_HEIGHT);
	m_pButtonNo->Move(20, m_pGroupBox->GetHeight() - 20 - CONTROL_HEIGHT);
	m_pGroupBox->Move((WINDOW_WIDTH - m_pGroupBox->GetWidth()) / 2, (WINDOW_HEIGHT - m_pGroupBox->GetHeight()) / 2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CQuestionBox::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
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

void CQuestionBox::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case BUTTON_YES:
			if (m_pNotificationWindow != NULL)
				m_pNotificationWindow->SendMessageToParent(this, m_iControlId, MS_DIALOG_YES);
			m_pParent->SendMessageToParent(this, m_iControlId, MS_DIALOG_YES);
			return;
		case BUTTON_NO:
			if (m_pNotificationWindow != NULL)
				m_pNotificationWindow->SendMessageToParent(this, m_iControlId, MS_DIALOG_NO);
			m_pParent->SendMessageToParent(this, m_iControlId, MS_DIALOG_NO);
			return;
		}
		break;
	}
	SallyAPI::GUI::CDialog::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}