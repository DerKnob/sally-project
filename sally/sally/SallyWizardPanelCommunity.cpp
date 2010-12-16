////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyWizardPanelCommunity.cpp
///
/// \brief	Implements the sally wizard panel community class. 
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

#include "SallyWizardPanelCommunity.h"

#define GUI_SHOW_COMMUNITY			50001

CSallyWizardPanelCommunity::CSallyWizardPanelCommunity(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CWizardPanel(parent, graphicId, pluginPath)
{
	m_pImageLogo->SetImageId(GUI_THEME_SALLY_COMMUNITY);

	m_pLabelHeader->SetText("Community");

	m_pLabelBox = new SallyAPI::GUI::CLabelBox(this, 20, WINDOW_BORDER_V + 50, 300, 80);
	m_pLabelBox->SetText("If you have already an account on the Sally community page than use these one to login.");
	m_pLabelBox->SetLocalised(true);
	this->AddChild(m_pLabelBox);

	m_pShowCommunityConfig = new SallyAPI::GUI::CButton(this, 20, 150, 200, CONTROL_HEIGHT, GUI_SHOW_COMMUNITY);
	m_pShowCommunityConfig->SetText("Configure Community");
	m_pShowCommunityConfig->SetImageId(GUI_THEME_SALLY_ICON_COMMUNITY);
	this->AddChild(m_pShowCommunityConfig);
}

CSallyWizardPanelCommunity::~CSallyWizardPanelCommunity(void)
{

}

void CSallyWizardPanelCommunity::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_SHOW_COMMUNITY:
			m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_SHOW_COMMUNITY_CONFIG);
			return;
		}
		break;
	}
	SallyAPI::GUI::CWizardPanel::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CSallyWizardPanelCommunity::SaveConfig()
{
}