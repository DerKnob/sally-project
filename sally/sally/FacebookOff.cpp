////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\FacebookOff.cpp
///
/// \brief	Implements the facebook off class. 
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

#include "FacebookOff.h"

CFacebookOff::CFacebookOff(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CForm(parent, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0)
{
	m_pLabelBoxWelcome = new SallyAPI::GUI::CLabelBox(this, (WINDOW_WIDTH - 400) / 2, (WINDOW_HEIGHT - 60) / 2, 400, 60);
	m_pLabelBoxWelcome->SetText("To use this application you must have the Facebook enabled.");
	m_pLabelBoxWelcome->SetAlign(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	this->AddChild(m_pLabelBoxWelcome);

	m_pShowFacebookConfig = new SallyAPI::GUI::CButton(this, (WINDOW_WIDTH - 200) / 2, (WINDOW_HEIGHT - 60) / 2 + 80, 200, CONTROL_HEIGHT, MS_SALLY_SHOW_FACEBOOK_CONFIG);
	m_pShowFacebookConfig->SetText("Configure Facebook");
	m_pShowFacebookConfig->SetImageId(GUI_THEME_SALLY_ICON_FACEBOOK);
	this->AddChild(m_pShowFacebookConfig);
}

CFacebookOff::~CFacebookOff()
{
}

void CFacebookOff::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case MS_SALLY_SHOW_FACEBOOK_CONFIG:
			m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_FACEBOOK_CONFIG);
			return;
		}
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}