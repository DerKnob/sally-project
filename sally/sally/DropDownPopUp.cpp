////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\DropDownPopUp.cpp
///
/// \brief	Implements the drop down pop up class. 
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
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU General Public License for more details.
/// 
/// You should have received a copy of the GNU General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DropDownPopUp.h"

CDropDownPopUp::CDropDownPopUp(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CContextMenuPopUp(parent, 0, "", 0, 0)
{
	int width = WINDOW_WIDTH / 2 - 60;
	int height = WINDOW_HEIGHT / 2 - 60;

	height = height - ((height - 40) % 30);

	m_pContextMenu->Resize(width, height);

	m_pListView = new SallyAPI::GUI::CListView(m_pContextMenu, 20, 20, width - 40, height - 40, 0, 0);
	m_pListView->ShowScrollbarIfNotScrollable(false);
	m_pContextMenu->AddChild(m_pListView);	
}

CDropDownPopUp::~CDropDownPopUp()
{
}

void CDropDownPopUp::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_THEME_SALLY_CLOSE:
			m_pParent->SendMessageToParent(this, 0, GUI_DROPDOWN_CANCELED);
			return;
		}
		break;
	case GUI_LISTVIEW_ITEM_CLICKED:
		OnCommandSelected(messageParameter);
		return;
	}
	SallyAPI::GUI::CContextMenuPopUp::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CDropDownPopUp::OnCommandSelected(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::GUI::SendMessage::CParameterInteger* parameterInteger = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterInteger*> (messageParameter);
	if (parameterInteger == NULL)
		return;

	SallyAPI::GUI::CListViewItem* item = m_pListView->GetItem(parameterInteger->GetInteger());
	SallyAPI::GUI::SendMessage::CParameterString messageParameterString(item->GetIdentifier());

	m_pParent->SendMessageToParent(this, 0, GUI_DROPDOWN_SELECTED, &messageParameterString);
}

void CDropDownPopUp::SetList(std::vector<SallyAPI::GUI::CDropDownItem>& itemList, const std::string& selectedIdentifier, bool localized)
{
	m_pListView->Clear();

	// set the localized
	m_pListView->SetLocalised(localized);

	std::vector<SallyAPI::GUI::CDropDownItem>::iterator iter = itemList.begin();

	int i = 0;
	int selected = -1;
	while (iter != itemList.end())
	{
		SallyAPI::GUI::CDropDownItem dropDownItem = *iter;
		SallyAPI::GUI::CListViewItem listItem(dropDownItem.GetIdentifier(), dropDownItem.GetText(), dropDownItem.GetImageId());
		
		if (dropDownItem.IsLocalised() == SallyAPI::GUI::DROPDOWN_LOCALISATION_FROM_PARENT)
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FROM_PARENT);
		else if (dropDownItem.IsLocalised() == SallyAPI::GUI::DROPDOWN_LOCALISATION_FALSE)
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_FALSE);
		else
			listItem.SetLocalised(SallyAPI::GUI::LISTVIEW_LOCALISATION_TRUE);
		
		m_pListView->AddItem(listItem);

		if (selectedIdentifier.compare(dropDownItem.GetIdentifier()) == 0)
			selected = i;

		++i;
		++iter;
	}

	// now activate the selected and scroll to this element
	if (selected != -1)
	{
		int elements = m_pListView->GetMaxDisplayedElements();
		m_pListView->SetActive(selected);
		m_pListView->SetStartItem(selected - (elements / 2) + 1);
	}	

	if (m_pListView->GetListSize() < m_pListView->GetMaxDisplayedElements())
	{
		m_pContextMenu->Resize(m_pListView->GetWidth() + 20 + 20 - 30, (m_pListView->GetListSize() * LISTVIEW_ITEM_HEIGHT) + 20 + 20);
		m_pListView->Move(20, -10);
	}
	else
	{
		m_pContextMenu->Resize(m_pListView->GetWidth() + 20 + 20, m_pListView->GetHeight() + 20 + 20);
		m_pListView->Move(20, 20);
	}
}