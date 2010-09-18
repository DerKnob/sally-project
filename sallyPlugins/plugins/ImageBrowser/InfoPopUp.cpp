////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\InfoPopUp.cpp
///
/// \brief	Implements the information pop up class. 
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

#include "InfoPopUp.h"

CInfoPopUp::CInfoPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName)
	: SallyAPI::GUI::CPopUpWindow(parent, graphicId, explicidAppName)
{
	m_pBackground = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 500) / 2, (WINDOW_HEIGHT - 280) / 2, 500, 280);
	this->AddChild(m_pBackground);

	m_pIcon = new SallyAPI::GUI::CImageBox(m_pBackground, -20, -20, 64, 64);
	m_pIcon->SetImageId(GetGraphicId());
	m_pBackground->AddChild(m_pIcon);

	m_pInfoViewTitle = new SallyAPI::GUI::CLabel(m_pBackground, 55, 20, 500 - 55 - 20);
	m_pInfoViewTitle->SetLocalised(false);
	m_pInfoViewTitle->SetFont("big3.font");
	m_pBackground->AddChild(m_pInfoViewTitle);

	m_pInfoViewLabel[0] = new SallyAPI::GUI::CLabel(m_pBackground, 20, 60, 100);
	m_pInfoViewLabel[0]->SetText("Resolution:");
	m_pBackground->AddChild(m_pInfoViewLabel[0]);

	m_pInfoViewLabel[1] = new SallyAPI::GUI::CLabel(m_pBackground, 20, 90, 100);
	m_pInfoViewLabel[1]->SetText("Size:");
	m_pBackground->AddChild(m_pInfoViewLabel[1]);

	m_pInfoViewLabel[2] = new SallyAPI::GUI::CLabel(m_pBackground, 20, 120, 100);
	m_pInfoViewLabel[2]->SetText("Create Date:");
	m_pBackground->AddChild(m_pInfoViewLabel[2]);

	m_pInfoViewLabel[3] = new SallyAPI::GUI::CLabel(m_pBackground, 20, 170, 100);
	m_pInfoViewLabel[3]->SetText("Filename:");
	m_pBackground->AddChild(m_pInfoViewLabel[3]);

	m_pInfoViewDesciption[0] = new SallyAPI::GUI::CLabelBox(m_pBackground, 120, 60, 500 - 140 - 20, CONTROL_HEIGHT);
	m_pInfoViewDesciption[0]->SetLocalised(false);
	m_pInfoViewDesciption[0]->SetAlign(DT_LEFT | DT_VCENTER);
	m_pBackground->AddChild(m_pInfoViewDesciption[0]);

	m_pInfoViewDesciption[1] = new SallyAPI::GUI::CLabelBox(m_pBackground, 120, 90, 500 - 140 - 20, CONTROL_HEIGHT);
	m_pInfoViewDesciption[1]->SetLocalised(false);
	m_pInfoViewDesciption[1]->SetAlign(DT_LEFT | DT_VCENTER);
	m_pBackground->AddChild(m_pInfoViewDesciption[1]);

	m_pInfoViewDesciption[2] = new SallyAPI::GUI::CLabelBox(m_pBackground, 120, 120, 500 - 140 - 20, CONTROL_HEIGHT);
	m_pInfoViewDesciption[2]->SetLocalised(false);
	m_pInfoViewDesciption[2]->SetAlign(DT_LEFT | DT_VCENTER);
	m_pBackground->AddChild(m_pInfoViewDesciption[2]);

	m_pInfoViewDesciption[3] = new SallyAPI::GUI::CLabelBox(m_pBackground, 20, 200, 500 - 20 - 20, CONTROL_HEIGHT * 2);
	m_pInfoViewDesciption[3]->SetLocalised(false);
	m_pBackground->AddChild(m_pInfoViewDesciption[3]);
}

CInfoPopUp::~CInfoPopUp()
{
}

void CInfoPopUp::Update(const std::string& file, SallyAPI::GUI::CPicture* picture)
{
	m_pInfoViewTitle->SetText(SallyAPI::String::PathHelper::GetFileFromPath(file));

	m_pInfoViewDesciption[1]->SetText(SallyAPI::File::FileHelper::GetFormatedFileSize(file));
	m_pInfoViewDesciption[2]->SetText(SallyAPI::File::FileHelper::GetFormatedFileWriteDate(file));
	m_pInfoViewDesciption[3]->SetText(file);

	std::string info;
	info.append(SallyAPI::String::StringHelper::ConvertToString(picture->GetWidth()));
	info.append(" x ");
	info.append(SallyAPI::String::StringHelper::ConvertToString(picture->GetHeight()));
	m_pInfoViewDesciption[0]->SetText(info);
}