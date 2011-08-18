////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\InfoPopup.cpp
///
/// \brief	Implements the information popup class. 
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

#include "InfoPopup.h"

#define GUI_BLEND_OUT	50001

CInfoPopup::CInfoPopup(SallyAPI::GUI::CGUIBaseObject* parent)
	: SallyAPI::GUI::CPopUpWindow(parent, 0, ""), m_pIdCounter(1)
{
	m_pBackgroundGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 400) / 2, -20, 400, 100);
	this->AddChild(m_pBackgroundGroupBox);
	
	m_pIcon = new SallyAPI::GUI::CImageBox(m_pBackgroundGroupBox, 10, 30, 64, 64);
	m_pBackgroundGroupBox->AddChild(m_pIcon);

	m_pText = new SallyAPI::GUI::CLabel(m_pBackgroundGroupBox, 10 + 80, 30, 310);
	m_pText->SetFont("infopopup.title.font");
	m_pText->SetLocalised(false);
	m_pBackgroundGroupBox->AddChild(m_pText);

	m_pDescription = new SallyAPI::GUI::CLabelBox(m_pBackgroundGroupBox, 10 + 80, 30 + CONTROL_HEIGHT, 310, 40);
	m_pDescription->SetFont("infopopup.text.font");
	m_pDescription->SetAutoResize(true);
	m_pDescription->SetLocalised(false);
	m_pBackgroundGroupBox->AddChild(m_pDescription);

	m_pBackground->SetImageId(GUI_NO_IMAGE);
	
	m_pButtonClose->Visible(false);
	m_tBlendOutTimer = new SallyAPI::GUI::CTimer(5, this, GetGraphicId(), GUI_BLEND_OUT);

	this->Move(0, -80);
}

CInfoPopup::~CInfoPopup()
{
	m_tBlendOutTimer->WaitForStop();
	SafeDelete(m_tBlendOutTimer);
}

void CInfoPopup::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_CONTROL_BLENDED:
		OnCommandBlended();
		return;
	case GUI_BLEND_OUT:
		OnCommandBlendOut();
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

int CInfoPopup::AddItem(SallyAPI::GUI::SendMessage::CParameterInfoPopup& parameter)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::System::COption* option = config->GetOption();

	if (!option->GetPropertyBool("sally", "showPopupInfos", true))
		return 0;

	SallyAPI::System::CAutoLock lock(&m_Lock);

	++m_pIdCounter;
	parameter.SetId(m_pIdCounter);
	m_vParameterInfoList.push_back(parameter);

	ShowNext();

	return m_pIdCounter;
}

void CInfoPopup::ShowNext()
{
	if ((m_vParameterInfoList.size() < 1) || (this->IsVisible()))
		return;

	this->Visible(true);
	// BlendIn();

	this->BlendAnimated(255, 800, false);
	this->MoveAnimated(0, 0, 500, false);

	SallyAPI::GUI::SendMessage::CParameterInfoPopup infoPopUp = m_vParameterInfoList.at(0);

	if (infoPopUp.GetPicture() != NULL)
		m_pIcon->SetPicture(infoPopUp.GetPicture());
	else
		m_pIcon->SetImageId(infoPopUp.GetIcon());

	// add the data
	m_pText->SetText(infoPopUp.GetText());
	m_pDescription->SetText(infoPopUp.GetDescription());

	ResizeGroupBox();

	m_tBlendOutTimer->Start();
}

void CInfoPopup::ResizeGroupBox()
{
	// resize groupbox
	int height = 30 + CONTROL_HEIGHT + m_pDescription->GetHeight() + 10;
	if (height < 30 + 10 + 64 + 10)
		height = 30 + 10 + 64 + 10;

	m_pBackgroundGroupBox->Resize(m_pBackgroundGroupBox->GetWidth(), height);
}

void CInfoPopup::OnCommandBlendOut()
{
	//BlendOut();

	this->BlendAnimated(0, 800, false);
	this->MoveAnimated(0, -80, 500, false);

	m_tBlendOutTimer->Stop();
}

void CInfoPopup::OnCommandBlended()
{
	if (this->GetAlphaBlending() == 0)
	{
		SallyAPI::System::CAutoLock lock(&m_Lock);

		this->Visible(false);

		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::System::COption* option = config->GetOption();

		if (!option->GetPropertyBool("sally", "showPopupInfos", true))
		{
			m_vParameterInfoList.clear();
		}
		else
		{
			if (m_vParameterInfoList.size() > 0)
				m_vParameterInfoList.erase(m_vParameterInfoList.begin());
			ShowNext();
		}
	}
}

void CInfoPopup::CalculateIconPosition(SallyAPI::GUI::CPicture* picture, int i)
{
	int x = 0;
	int y = 0;
	if (picture != NULL)
	{
		int width = 0;
		int height = 0;

		int pictureWidth = picture->GetWidth();
		int pictureHeight = picture->GetHeight();

		if ((pictureWidth != 0) && (pictureHeight != 0))
		{
			SallyAPI::GUI::GUIHelper::CalculateImageSize(pictureWidth, pictureHeight,
				64, 64, x, y, width, height);
		}
		else
		{
			// the picture width is not set... try the texture
			SallyAPI::Core::CTexture* texture = picture->GetTexture();
			if (texture != NULL)
			{
				pictureHeight = texture->GetHeight();
				pictureWidth = texture->GetWidth();

				SallyAPI::GUI::GUIHelper::CalculateImageSize(pictureWidth, pictureHeight,
					64, 64, x, y, width, height);
			}
			else
			{
				// fallback
				width = 64;
				height = 64;
			}
		}
		m_pIcon->Resize(width, height);
	}
	else
	{
		m_pIcon->Resize(64, 64);
	}

	int postion = (i - 2) * -1;
	
	x += ((WINDOW_WIDTH - 370) / 2);
	y += ((postion * 70) + 8);

	m_pIcon->Move(x, y);
}

void CInfoPopup::RemoveItem(int id)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	std::vector<SallyAPI::GUI::SendMessage::CParameterInfoPopup>::iterator iter = m_vParameterInfoList.begin();

	int i = 0;
	while (iter != m_vParameterInfoList.end())
	{
		SallyAPI::GUI::SendMessage::CParameterInfoPopup infoPopUp = *iter;

		if (infoPopUp.GetId() == id)
		{
			// first item in list? than it's displayed
			if (i == 0)
			{
				m_pIcon->SetPicture(NULL);
				m_pIcon->SetImageId(0);

				m_tBlendOutTimer->ExecuteNow();
			}
			m_vParameterInfoList.erase(iter);
			iter = m_vParameterInfoList.end();
		}
		else
		{
			++i;
			++iter;
		}
	}
}

void CInfoPopup::Render()
{
	// is needed to render the normal way
	// and not the popup way
	SallyAPI::GUI::CAppBase::Render();
}

void CInfoPopup::SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case MS_SALLY_SALLY_THEME_CHANGED:
		ResizeGroupBox();
		break;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToChilds(reporter, reporterId, messageId, messageParameter);
}