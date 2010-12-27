////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\SallyWizardPanelFacebook.cpp
///
/// \brief	Implements the sally wizard panel facebook class. 
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

#include "SallyWizardPanelFacebook.h"

#define GUI_SHOW_FACEBOOK_CONFIG			50001

CSallyWizardPanelFacebook::CSallyWizardPanelFacebook(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CWizardPanel(parent, graphicId, pluginPath)
{
	m_pImageLogo->SetImageId(GUI_THEME_SALLY_FACEBOOK);

	m_pLabelHeader->SetText("Facebook");

	m_pLabelBox = new SallyAPI::GUI::CLabelBox(this, 20, WINDOW_BORDER_V + 50, WIZARD_PANEL_WIDTH - 40, 140);
	m_pLabelBox->SetText("You can connect Sally to your Facebook profile.\n\nIf Facebook is enabled you can for example use the 'I Like' buttons in the applications.\nAlso the Facebook app can be used and you can for example see what our friends are listening to while using Sally or you can post a status message to your Facebook account.");
	m_pLabelBox->SetLocalised(true);
	this->AddChild(m_pLabelBox);

	m_pShowFacebookConfig = new SallyAPI::GUI::CButton(this, 20, 220, 250, CONTROL_HEIGHT, GUI_SHOW_FACEBOOK_CONFIG);
	m_pShowFacebookConfig->SetText("Configure Facebook");
	m_pShowFacebookConfig->SetImageId(GUI_THEME_SALLY_ICON_FACEBOOK);
	this->AddChild(m_pShowFacebookConfig);
}

CSallyWizardPanelFacebook::~CSallyWizardPanelFacebook(void)
{

}

void CSallyWizardPanelFacebook::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_SHOW_FACEBOOK_CONFIG:
			m_pParent->SendMessageToParent(this, GetGraphicId(), MS_SALLY_SHOW_FACEBOOK_CONFIG);
			return;
		}
		break;
	}
	SallyAPI::GUI::CWizardPanel::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CSallyWizardPanelFacebook::SaveConfig()
{
}