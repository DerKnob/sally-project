////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\LyricsPopUp.cpp
///
/// \brief	Implements the lyrics pop up class. 
///
/// \author	Christian Knobloch
/// \date	08.07.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
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

#include "LyricsPopUp.h"

CLyricsPopUp::CLyricsPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName)
	: SallyAPI::GUI::CPopUpWindow(parent, graphicId, explicidAppName)
{
	SetCloseOnClick(true);

	int height = WINDOW_HEIGHT - 60;
	int width = height;

	m_pBackground = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - width) / 2, (WINDOW_HEIGHT - height) / 2, width, height);
	m_pBackground->SetAlwaysHandleInput(true);
	this->AddChild(m_pBackground);

	m_pIcon = new SallyAPI::GUI::CImageBox(m_pBackground, -20, -20, 64, 64);
	m_pIcon->SetImageId(GetGraphicId());
	m_pBackground->AddChild(m_pIcon);

	m_pTitle = new SallyAPI::GUI::CLabel(m_pBackground, 55, 20, width - 55 - 20);
	m_pTitle->SetFont("big3.font");
	m_pTitle->SetLocalised(false);
	m_pBackground->AddChild(m_pTitle);

	m_pArtist = new SallyAPI::GUI::CLabel(m_pBackground, 55, 50, width - 55 - 20);
	m_pArtist->SetLocalised(false);
	m_pArtist->SetFont("big2.font");
	m_pBackground->AddChild(m_pArtist);

	m_pText = new SallyAPI::GUI::CLabelBox(m_pBackground, 20, 90, width - 20 - 20, height - 90 - 20, true);
	m_pText->SetLocalised(false);
	m_pText->SetAlign(DT_CENTER | DT_WORDBREAK);
	m_pText->SetFont("big1.font");
	m_pBackground->AddChild(m_pText);

	m_pLoading = new SallyAPI::GUI::CWorking(m_pBackground, (width - 128) / 2, height / 2, SallyAPI::GUI::WORKING_BIG);
	m_pLoading->Visible(false);
	m_pBackground->AddChild(m_pLoading);
}

CLyricsPopUp::~CLyricsPopUp()
{
	while (m_ThreadPool.GetStatus() == SallyAPI::System::THREAD_RUNNING)
	{
		Sleep(1000);
	}
}

void CLyricsPopUp::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId,
									   SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_APP_LYRICS_LOADED:
		LyricsLoaded(messageParameter);
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CLyricsPopUp::GetLyric(const std::string& artist, const std::string& title)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	// set text to controls
	std::string temp = "by ";
	temp.append(artist);

	m_pArtist->SetText(temp);
	m_pTitle->SetText(title);
	m_pText->SetText("");

	// start lyric getter
	CLyricGetter* lyricGetter = new CLyricGetter();
	lyricGetter->SetStaticValues((dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent)), this);
	lyricGetter->SetValues(artist, title);

	lyricGetter->Start();

	m_ThreadPool.AddThread(lyricGetter);
	m_ThreadPool.Start();

	m_pLastThreadId = lyricGetter->GetId();

	m_pLoading->Visible(true);
}

void CLyricsPopUp::LyricsLoaded(SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	SallyAPI::System::CAutoLock lock(&m_Lock);

	m_pLoading->Visible(false);

	SallyAPI::GUI::SendMessage::CParameterKeyValue* message = dynamic_cast<SallyAPI::GUI::SendMessage::CParameterKeyValue*> (messageParameter);
	if (message == NULL)
		return;

	if (m_pLastThreadId.compare(message->GetKey()) == 0)
		m_pText->SetText(message->GetValue());
}