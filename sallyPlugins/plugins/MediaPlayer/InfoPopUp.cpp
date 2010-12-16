////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\InfoPopUp.cpp
///
/// \brief	Implements the information pop up class. 
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

CInfoPopUp::CInfoPopUp(SallyAPI::GUI::CGUIBaseObject *parent, int graphicId, const std::string& explicidAppName)
	: SallyAPI::GUI::CPopUpWindow(parent, graphicId, explicidAppName)
{
	m_pBackground = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 700) / 2, (WINDOW_HEIGHT - 300) / 2, 700, 300);
	this->AddChild(m_pBackground);

	m_pIcon = new SallyAPI::GUI::CImageBox(m_pBackground, -20, -20, 64, 64);
	m_pIcon->SetImageId(GetGraphicId());
	m_pBackground->AddChild(m_pIcon);

	m_pArtist = new SallyAPI::GUI::CLabel(m_pBackground, 55, 20, 700 - 55 - 20);
	m_pArtist->SetFont("big2.font");
	m_pArtist->SetLocalised(false);
	m_pBackground->AddChild(m_pArtist);

	m_pTitle = new SallyAPI::GUI::CLabel(m_pBackground, 20, 60, 480);
	m_pTitle->SetFont("big2.font");
	m_pTitle->SetLocalised(false);
	m_pBackground->AddChild(m_pTitle);

	m_pAlbum = new SallyAPI::GUI::CLabel(m_pBackground, 20, 100, 480);
	m_pAlbum->SetFont("big2.font");
	m_pAlbum->SetLocalised(false);
	m_pBackground->AddChild(m_pAlbum);

	m_pRating = new SallyAPI::GUI::CRating(m_pBackground, 700 - (30 * 5) - 20, 60);
	m_pRating->Visible(false);
	m_pBackground->AddChild(m_pRating);

	m_pRemoveRating = new SallyAPI::GUI::CButton(m_pBackground, 700 - 200 - 20, 100, 200, CONTROL_HEIGHT, GUI_APP_REMOVE_RATING);
	m_pRemoveRating->SetImageId(GUI_THEME_SALLY_ICON_RATING_OFF);
	m_pRemoveRating->SetText("Remove Rating");
	m_pRemoveRating->Visible(false);
	m_pBackground->AddChild(m_pRemoveRating);

	for (int i = 0; i < 7; i++)
	{
		int iX = 20 + 230 + 100;
		if ((i - 1) % 2)
			iX = 20;

		int iY = (i / 2 * 30) + 160;


		m_pDescriptionLabel[i] = new SallyAPI::GUI::CLabel(m_pBackground, iX, iY, 90);
		m_pDescriptionLabel[i]->SetBold(false);
		m_pBackground->AddChild(m_pDescriptionLabel[i]);

		m_pStatusLabel[i] = new SallyAPI::GUI::CLabel(m_pBackground, iX + 100, iY, 110);
		m_pStatusLabel[i]->SetLocalised(false);
		m_pStatusLabel[i]->SetBold(true);
		m_pBackground->AddChild(m_pStatusLabel[i]);
	}

	m_pStatusLabel[6]->Resize(560, CONTROL_HEIGHT);

	m_pDescriptionLabel[0]->SetText("Track:");
	m_pDescriptionLabel[1]->SetText("Genre:");
	m_pDescriptionLabel[2]->SetText("Year:");
	m_pDescriptionLabel[3]->SetText("BitRate:");
	m_pDescriptionLabel[4]->SetText("Composer:");
	m_pDescriptionLabel[5]->SetText("Time Played:");
	m_pDescriptionLabel[6]->SetText("Filename:");
}

CInfoPopUp::~CInfoPopUp()
{
}

void CInfoPopUp::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_REMOVE_RATING:
			OnCommandRemoveRating();
			return;
		}
		break;
	case GUI_RATING_CHANGED:
		OnCommandRatingChanged();
		break;

	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CInfoPopUp::OnCommandRemoveRating()
{
	m_pRating->SetRating(0);

	SallyAPI::GUI::SendMessage::CParameterInteger integer(0);
	m_pParent->SendMessageToParent(this, GUI_APP_INFO_RATING_CHANGED, GUI_RATING_CHANGED, &integer);
}

void CInfoPopUp::OnCommandRatingChanged()
{
	SallyAPI::GUI::SendMessage::CParameterInteger integer(m_pRating->GetRating());
	m_pParent->SendMessageToParent(this, GUI_APP_INFO_RATING_CHANGED, GUI_RATING_CHANGED, &integer);
}

void CInfoPopUp::UpdateMp3(CAudioFile* mp3File, const std::string& timeplayed, int rating)
{
	MP3FileInfo* fileInfo = mp3File->GetMp3Tag();

	if (fileInfo != NULL)
	{
		m_pArtist->SetText(fileInfo->GetSzArtist());
		m_pAlbum->SetText(fileInfo->GetSzTitle());
		m_pTitle->SetText(fileInfo->GetSzAlbum());
		m_pStatusLabel[0]->SetText(fileInfo->GetSzTrack());
		m_pStatusLabel[1]->SetText(CAudioFile::GetMp3Genre(fileInfo->GetSzGenre()));
		m_pStatusLabel[2]->SetText(fileInfo->GetSzYear());
		m_pStatusLabel[3]->SetText(fileInfo->GetSzBitRate());
		m_pStatusLabel[5]->SetText(fileInfo->GetSzComposer());
	}
	else
	{
		m_pArtist->SetText("");
		m_pAlbum->SetText("");
		m_pTitle->SetText("");
		m_pStatusLabel[0]->SetText("");
		m_pStatusLabel[1]->SetText("");
		m_pStatusLabel[2]->SetText("");
		m_pStatusLabel[3]->SetText("");
		m_pStatusLabel[4]->SetText("");
	}

	m_pStatusLabel[5]->SetText(timeplayed);
	m_pStatusLabel[6]->SetText(mp3File->GetFilename());

	UpdateRating(rating);
}

void CInfoPopUp::UpdateVideo(CVideoFile* videoFile, const std::string& timeplayed, int rating)
{
	m_pArtist->SetText(SallyAPI::String::PathHelper::GetFileFromPath(videoFile->GetFilename()));
	m_pAlbum->SetText("");
	m_pTitle->SetText("");
	m_pStatusLabel[0]->SetText("");
	m_pStatusLabel[1]->SetText("");
	m_pStatusLabel[2]->SetText("");
	m_pStatusLabel[3]->SetText("");
	m_pStatusLabel[4]->SetText("");

	m_pStatusLabel[5]->SetText(timeplayed);
	m_pStatusLabel[6]->SetText(videoFile->GetFilename());

	UpdateRating(rating);
}

void CInfoPopUp::UpdateRating(int rating)
{
	if (rating != -1)
	{
		m_pRemoveRating->Visible(true);
		m_pRating->Visible(true);
		m_pRating->SetRating(rating);
	}
	else
	{
		m_pRemoveRating->Visible(false);
		m_pRating->Visible(false);
	}
}