////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\WorkingWindow.cpp
///
/// \brief	Implements the working Windows Form. 
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

#include "WorkingWindow.h"

CWorkingWindow::CWorkingWindow(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, "")
{
	SetBlendIn(SallyAPI::GUI::POPUP_BLEND_IN_CENTER);

	m_pLabel = new SallyAPI::GUI::CLabel(this, 0, (WINDOW_HEIGHT / 2) + 30, WINDOW_WIDTH);
	m_pLabel->SetAlign(DT_VCENTER | DT_CENTER);
	m_pLabel->SetText("working - please wait");
	m_pLabel->SetFont("working.font");
	this->AddChild(m_pLabel);

	m_pLogo = new SallyAPI::GUI::CWorking(this, (WINDOW_WIDTH / 2) - 32, (WINDOW_HEIGHT / 2) - 50, SallyAPI::GUI::WORKING_MEDIUM);
	this->AddChild(m_pLogo);

	m_pBackground->SetImageId(GUI_THEME_SALLY_POPUP_WORKING);

	m_pButtonClose->Visible(false);
}

CWorkingWindow::~CWorkingWindow(void)
{
}

void CWorkingWindow::Timer(float timeDelta)
{
	SallyAPI::GUI::CPopUpWindow::Timer(timeDelta);

	int alpha = m_pBackground->GetAlphaBlending();

	if (alpha == 0)
		Visible(false);
	else
		Visible(true);
}