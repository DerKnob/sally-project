////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\OpenDialog.cpp
///
/// \brief	Implements the open Dialog. 
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

#include "OpenDialog.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	COpenDialog::COpenDialog(SallyAPI::GUI::CGUIBaseObject* parent) :SallyAPI::GUI::CDialogBox(parent)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
////////////////////////////////////////////////////////////////////////////////////////////////////

COpenDialog::COpenDialog(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CDialogBox(parent)
{
	m_pGroupBox->Move((WINDOW_WIDTH - 780) / 2, (WINDOW_HEIGHT - 580) / 2);
	m_pGroupBox->Resize(780, 580);

	m_pLableDialogText = new SallyAPI::GUI::CLabelBox(m_pGroupBox, 20, 20, 740, CONTROL_HEIGHT);
	m_pLableDialogText->SetLocalised(true);
	m_pGroupBox->AddChild(m_pLableDialogText);

	m_pButtonCancel = new SallyAPI::GUI::CButton(m_pGroupBox, 20, 580 - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, BUTTON_CANCEL, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonCancel->SetText("Cancel");
	m_pButtonCancel->SetImageId(GUI_THEME_SALLY_ICON_CANCEL);
	m_pGroupBox->AddChild(m_pButtonCancel);

	m_pFileBrowser = new SallyAPI::GUI::CFileBrowser(m_pGroupBox, 20, 20 + CONTROL_HEIGHT + 10, 740, 580 - 20 - CONTROL_HEIGHT - 10 - 20);
	m_pGroupBox->AddChild(m_pFileBrowser);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	COpenDialog::~COpenDialog()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

COpenDialog::~COpenDialog()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COpenDialog::SetValues(SallyAPI::GUI::SendMessage::CParameterOpenDialog& openDialog)
///
/// \brief	Sets the values. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	openDialog	The open dialog. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void COpenDialog::SetValues(SallyAPI::GUI::SendMessage::CParameterOpenDialog& openDialog)
{
	m_iControlId = openDialog.GetControlID();
	m_pNotificationWindow = openDialog.GetNotificationWindow();
	m_pLableDialogText->SetText(openDialog.GetDialogText());

	m_iActionCommand = openDialog.GetActionCommand();
	if (m_iActionCommand > 0)
		m_iActionCommand += 100;
	m_pFileBrowser->SetActionButtonCommand(m_iActionCommand);
	m_pFileBrowser->SetActionButtonText(openDialog.GetActionText());
	m_pFileBrowser->SetActionButtonImage(openDialog.GetActionImage());

	m_pFileBrowser->SetMimetypeList(openDialog.GetMimetypeList());
	m_pFileBrowser->SetShowHardDisks(true);
	m_pFileBrowser->SetShowSpecialFolders(openDialog.GetShowSpecialFolders());
	m_pFileBrowser->SetShowRemovableDisk(openDialog.GetShowRemovableDisk());
	m_pFileBrowser->SetStartFolders(openDialog.GetStartFolders());
	m_pFileBrowser->SetShowSubfolders(openDialog.GetShowSubfolders());

	if (openDialog.GetGoToFolder().length() > 0)
		m_pFileBrowser->SetFolder(openDialog.GetGoToFolder());
	else
		m_pFileBrowser->Reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COpenDialog::SendMessageToParent(CGUIBaseObject* reporter, int reporterId,
/// int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
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

void COpenDialog::SendMessageToParent(CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_LISTVIEW_ITEM_ACTION_CLICKED:
		OnCommandOk(messageParameter);
		break;
	case GUI_BUTTON_CLICKED:
		if (m_iActionCommand == reporterId)
		{
			OnCommandCompleteFolder();
			return;
		}

		switch (reporterId)
		{
		case BUTTON_CANCEL:
			m_pParent->SendMessageToParent(m_pNotificationWindow, m_iControlId, MS_SALLY_CANCEL_OPEN_FILE);
			return;
		}
		break;
	}
	SallyAPI::GUI::CDialog::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COpenDialog::OnCommandCompleteFolder()
///
/// \brief	Executes the command complete folder action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void COpenDialog::OnCommandCompleteFolder()
{
	SallyAPI::GUI::SendMessage::CParameterString messageString(m_pFileBrowser->GetCurrentFolder());

	m_pParent->SendMessageToParent(m_pNotificationWindow, m_iControlId, MS_SALLY_OK_OPEN_FILE, &messageString);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void COpenDialog::OnCommandOk(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Executes the command ok action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void COpenDialog::OnCommandOk(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);

	SallyAPI::GUI::CListView* listView = m_pFileBrowser->GetListView();
	SallyAPI::GUI::CListViewItem* listItem = listView->GetItem(parameterInteger->GetInteger());
	if (listItem == NULL)
		return;
	SallyAPI::GUI::SendMessage::CParameterString messageString(listItem->GetIdentifier());

	m_pParent->SendMessageToParent(m_pNotificationWindow, m_iControlId, MS_SALLY_OK_OPEN_FILE, &messageString);
}