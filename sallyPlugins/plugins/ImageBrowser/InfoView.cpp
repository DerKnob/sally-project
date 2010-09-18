////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\InfoView.cpp
///
/// \brief	Implements the info view class. 
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

#include "InfoView.h"

CInfoView::CInfoView(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y)
:SallyAPI::GUI::CForm(parent, x, y, 300, INFO_VIEW_HEIGHT)
{
	m_pInfoView = new SallyAPI::GUI::CGroupBox(this, 0, 0, 300, INFO_VIEW_HEIGHT);
	m_pInfoView->SetAlwaysHandleInput(true);
	this->AddChild(m_pInfoView);

	m_pInfoViewShowButton = new SallyAPI::GUI::CButton(m_pInfoView, 50, INFO_VIEW_HEIGHT - 20 - CONTROL_HEIGHT, 200, CONTROL_HEIGHT, GUI_APP_SHOW_INFO_VIEW);
	m_pInfoViewShowButton->SetImageId(GUI_THEME_SALLY_ICON_DOWN);
	m_pInfoViewShowButton->SetText("Show Infos");
	m_pInfoView->AddChild(m_pInfoViewShowButton);

	m_pInfoViewSmallImage = new SallyAPI::GUI::CImageBox(m_pInfoView, 300 - 80, 40, 100, 100);
	m_pInfoViewSmallImage->SetDiyplayType(SallyAPI::GUI::IMAGEBOX_DISPLAY_TYPE_SCALE);
	m_pInfoView->AddChild(m_pInfoViewSmallImage);

	m_pInfoViewTitle = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 40, 150);
	m_pInfoViewTitle->SetBold(true);
	m_pInfoView->AddChild(m_pInfoViewTitle);

	m_pInfoViewLabel[0] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 70, 80);
	m_pInfoViewLabel[0]->SetText("Resolution:");
	m_pInfoView->AddChild(m_pInfoViewLabel[0]);

	m_pInfoViewLabel[1] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 140, 80);
	m_pInfoViewLabel[1]->SetText("Size:");
	m_pInfoView->AddChild(m_pInfoViewLabel[1]);

	m_pInfoViewLabel[2] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 210, 80);
	m_pInfoViewLabel[2]->SetText("Create Date:");
	m_pInfoView->AddChild(m_pInfoViewLabel[2]);

	m_pInfoViewLabel[3] = new SallyAPI::GUI::CLabel(m_pInfoView, 20, 280, 80);
	m_pInfoViewLabel[3]->SetText("Filename:");
	m_pInfoView->AddChild(m_pInfoViewLabel[3]);

	m_pInfoViewDesciption[0] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 70 + CONTROL_HEIGHT, 300 - 60, CONTROL_HEIGHT);
	m_pInfoViewDesciption[0]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[0]);

	m_pInfoViewDesciption[1] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 140 + CONTROL_HEIGHT, 300 - 40, CONTROL_HEIGHT * 2);
	m_pInfoViewDesciption[1]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[1]);

	m_pInfoViewDesciption[2] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 210 + CONTROL_HEIGHT, 300 - 40, CONTROL_HEIGHT * 2);
	m_pInfoViewDesciption[2]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[2]);

	m_pInfoViewDesciption[3] = new SallyAPI::GUI::CLabelBox(m_pInfoView, 20, 280 + CONTROL_HEIGHT, 300 - 40, CONTROL_HEIGHT * 2);
	m_pInfoViewDesciption[3]->SetLocalised(false);
	m_pInfoView->AddChild(m_pInfoViewDesciption[3]);
}

CInfoView::~CInfoView()
{
}

void CInfoView::SetFileInfo(const std::string& file)
{
	m_pInfoViewDesciption[3]->SetText(file);
	m_pInfoViewTitle->SetText(SallyAPI::String::PathHelper::GetFileFromFolderString(file));
	m_pInfoViewSmallImage->SetPicture(NULL);
	m_pInfoViewDesciption[1]->SetText(SallyAPI::File::FileHelper::GetFormatedFileSize(file));
	m_pInfoViewDesciption[2]->SetText(SallyAPI::File::FileHelper::GetFormatedFileWriteDate(file));
}

void CInfoView::SetSmallPicture(SallyAPI::GUI::CPicture* picture)
{
	m_pInfoViewSmallImage->SetPicture(picture);
	m_pInfoViewSmallImage->SetAlphaBlending(0);
	m_pInfoViewSmallImage->BlendAnimated(255, 400, true);

	std::string info;
	info.append(SallyAPI::String::StringHelper::ConvertToString(picture->GetWidth()));
	info.append(" x ");
	info.append(SallyAPI::String::StringHelper::ConvertToString(picture->GetHeight()));
	m_pInfoViewDesciption[0]->SetText(info);
}

void CInfoView::BlendIn()
{
	m_pInfoViewShowButton->SetText("Show Infos");
	m_pInfoViewShowButton->SetImageId(GUI_THEME_SALLY_ICON_DOWN);
	m_pInfoViewShowButton->Enable(false);
}

void CInfoView::BlendOut()
{
	m_pInfoViewShowButton->SetText("Hide Infos");
	m_pInfoViewShowButton->SetImageId(GUI_THEME_SALLY_ICON_UP);
	m_pInfoViewShowButton->Enable(false);
}