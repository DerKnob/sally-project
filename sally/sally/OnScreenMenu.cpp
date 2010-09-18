////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\OnScreenMenu.cpp
///
/// \brief	Implements the on screen menu class. 
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

#include "OnScreenMenu.h"

COnScreenMenu::COnScreenMenu(SallyAPI::GUI::CGUIBaseObject* parent)
	: SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	m_pIcon = new SallyAPI::GUI::CImageBox(this, (WINDOW_WIDTH - 64) / 2, (WINDOW_HEIGHT - 64) / 2 - 15, 64, 64);
	this->AddChild(m_pIcon);

	m_pText = new SallyAPI::GUI::CLabel(this, (WINDOW_WIDTH - 120) / 2, (WINDOW_HEIGHT - CONTROL_HEIGHT) / 2 + 35, 120);
	m_pText->SetFont("onscreenmenu.font");
	m_pText->SetAlign(DT_CENTER | DT_VCENTER);
	this->AddChild(m_pText);

	m_pBackground->Move((WINDOW_WIDTH - 120) / 2, (WINDOW_HEIGHT - 120) / 2);
	m_pBackground->Resize(120, 120);
	m_pBackground->SetImageId(GUI_THEME_SALLY_POPUP_ONSCREENMENU);

	m_pButtonClose->Visible(false);

	this->Move(0, 0);
}

COnScreenMenu::~COnScreenMenu(void)
{
}

void COnScreenMenu::Render()
{
	SallyAPI::GUI::CAppBase::Render();
}

void COnScreenMenu::BlendIn()
{
	this->BlendAnimated(255, 800);
}

void COnScreenMenu::BlendOut()
{
	this->BlendAnimated(0, 800);
}

void COnScreenMenu::SetImageId(int icon)
{
	m_pIcon->SetImageId(icon);
}

void COnScreenMenu::SetText(const std::string& text)
{
	m_pText->SetText(text);
}

void COnScreenMenu::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_CONTROL_BLENDED:
		if (m_pBackground->GetAlphaBlending() == 0)
			Visible(false);
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}