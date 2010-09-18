////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\WindowLoading.cpp
///
/// \brief	Implements the window loading class. 
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

#include "WindowLoading.h"

CWindowLoading::CWindowLoading()
{
	m_pStatus = new SallyAPI::GUI::CLabel(this, 20, WINDOW_HEIGHT - 80, WINDOW_WIDTH - 40);
	m_pStatus->SetLocalised(false);
	m_pStatus->SetFont("loading.font");
	m_pStatus->Visible(false);
	this->AddChild(m_pStatus);

	m_pProcessbar = new SallyAPI::GUI::CProcessbar(this, 20, WINDOW_HEIGHT - 50, WINDOW_WIDTH - 40);
	m_pProcessbar->SetMaxPosition(100);
	this->AddChild(m_pProcessbar);

	this->SendMessageToParent(this, 0, GUIM_START_LOADING);
}

CWindowLoading::~CWindowLoading()
{
}

void CWindowLoading::RenderControl()
{
	int x = (WINDOW_WIDTH - MAX_WIDTH) / 2;
	int y = (WINDOW_HEIGHT - MAX_HEIGHT) / 2;
	DrawImage(GUI_THEME_SALLY_LOADING, x, y);

	SallyAPI::GUI::CWindow::RenderControl();
}

void CWindowLoading::SetStatus(const std::string& status)
{
	m_pStatus->SetText(status);
}

void CWindowLoading::SetProcessbar(int i)
{
	m_pProcessbar->SetPosition(i);
}

void CWindowLoading::UpdateProcessbar()
{
	int i = m_pProcessbar->GetPosition();
	m_pProcessbar->SetPosition(i + 3);
}