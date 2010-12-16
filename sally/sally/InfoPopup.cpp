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
	for (int i = 0; i < 1; ++i)
	{
		int posX = (WINDOW_WIDTH - 370) / 2;
		m_pIcon[i] = new SallyAPI::GUI::CImageBox(this, posX + 5, (i * 70) + 8, 64, 64);
		this->AddChild(m_pIcon[i]);

		m_pText[i] = new SallyAPI::GUI::CLabel(this, posX + 5 + 70, (i * 70) + 8, 300);
		m_pText[i]->SetFont("infopopup.title.font");
		m_pText[i]->SetLocalised(false);
		this->AddChild(m_pText[i]);

		m_pDescription[i] = new SallyAPI::GUI::CLabelBox(this, posX + 5 + 70, (i * 70) + 6 + CONTROL_HEIGHT, 300, 38);
		m_pDescription[i]->SetFont("infopopup.text.font");
		m_pDescription[i]->SetLocalised(false);
		this->AddChild(m_pDescription[i]);
	}

	m_pBackground->Move((WINDOW_WIDTH - 380) / 2, 0);
	m_pBackground->Resize(380, 80);
	m_pBackground->SetImageId(GUI_THEME_SALLY_INFO_POPUP);
	
	m_pButtonClose->Visible(false);

	InitializeCriticalSection(&m_critSectLock);

	m_tBlendOutTimer = new SallyAPI::GUI::CTimer(5, this, GetGraphicId(), GUI_BLEND_OUT);

	this->Move(0, -80);
}

CInfoPopup::~CInfoPopup()
{
	m_tBlendOutTimer->WaitForStop();
	SafeDelete(m_tBlendOutTimer);

	DeleteCriticalSection(&m_critSectLock);
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

	if (!option->GetPropertyBool("sally", "showpopupinfos", true))
		return 0;

	EnterCriticalSection(&m_critSectLock);

	++m_pIdCounter;
	parameter.SetId(m_pIdCounter);
	m_vParameterInfoList.push_back(parameter);

	ShowNext();

	LeaveCriticalSection(&m_critSectLock);

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

	int i = 0;
	SallyAPI::GUI::SendMessage::CParameterInfoPopup infoPopUp = m_vParameterInfoList.at(i);

	if (infoPopUp.GetPicture() != NULL)
		m_pIcon[i]->SetPicture(infoPopUp.GetPicture());
	else
		m_pIcon[i]->SetImageId(infoPopUp.GetIcon());

	// add the data
	m_pText[i]->SetText(infoPopUp.GetText());
	m_pDescription[i]->SetText(infoPopUp.GetDescription());

	m_tBlendOutTimer->Start();
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
		EnterCriticalSection(&m_critSectLock);

		this->Visible(false);

		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::System::COption* option = config->GetOption();

		if (!option->GetPropertyBool("sally", "showpopupinfos", true))
		{
			m_vParameterInfoList.clear();
		}
		else
		{
			if (m_vParameterInfoList.size() > 0)
				m_vParameterInfoList.erase(m_vParameterInfoList.begin());
			ShowNext();
		}

		LeaveCriticalSection(&m_critSectLock);
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
		m_pIcon[i]->Resize(width, height);
	}
	else
	{
		m_pIcon[i]->Resize(64, 64);
	}

	int postion = (i - 2) * -1;
	
	x += ((WINDOW_WIDTH - 370) / 2);
	y += ((postion * 70) + 8);

	m_pIcon[i]->Move(x, y);
}

void CInfoPopup::RemoveItem(int id)
{
	EnterCriticalSection(&m_critSectLock);

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
				m_pIcon[0]->SetPicture(NULL);
				m_pIcon[0]->SetImageId(0);

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
	LeaveCriticalSection(&m_critSectLock);
}

void CInfoPopup::Render()
{
	// is needed to render the normal way
	// and not the popup way
	SallyAPI::GUI::CAppBase::Render();
}