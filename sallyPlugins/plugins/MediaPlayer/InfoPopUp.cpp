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

#define GUI_SAVE_BUTTON		50000
#define GUI_CANCEL_BUTTON	50001
#define GUI_EDIT_BUTTON		50002

CInfoPopUp::CInfoPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName)
	: SallyAPI::GUI::CPopUpWindow(parent, graphicId, explicidAppName)
{
	SetCloseOnClick(true);

	m_pBackground = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 700) / 2, (WINDOW_HEIGHT - 340) / 2, 700, 340);
	m_pBackground->SetAlwaysHandleInput(true);
	this->AddChild(m_pBackground);

	m_pIcon = new SallyAPI::GUI::CImageBox(m_pBackground, -20, -20, 64, 64);
	m_pIcon->SetImageId(GetGraphicId());
	m_pBackground->AddChild(m_pIcon);

	/*******************************************/

	m_pFormView = new SallyAPI::GUI::CForm(this, m_pBackground->GetPositionX(), m_pBackground->GetPositionY(),
		m_pBackground->GetWidth(), m_pBackground->GetHeight());
	this->AddChild(m_pFormView);

	m_pArtist = new SallyAPI::GUI::CLabel(m_pFormView, 55, 20, 700 - 55 - 20);
	m_pArtist->SetFont("big2.font");
	m_pArtist->SetLocalised(false);
	m_pFormView->AddChild(m_pArtist);

	m_pTitle = new SallyAPI::GUI::CLabel(m_pFormView, 20, 60, 480);
	m_pTitle->SetFont("big2.font");
	m_pTitle->SetLocalised(false);
	m_pFormView->AddChild(m_pTitle);

	m_pAlbum = new SallyAPI::GUI::CLabel(m_pFormView, 20, 100, 480);
	m_pAlbum->SetLocalised(false);
	m_pFormView->AddChild(m_pAlbum);

	m_pRating = new SallyAPI::GUI::CRating(m_pFormView, 700 - (30 * 5) - 20, 60);
	m_pRating->Visible(false);
	m_pFormView->AddChild(m_pRating);

	m_pRemoveRating = new SallyAPI::GUI::CButton(m_pFormView, 700 - 220 - 20, 100, 220, CONTROL_HEIGHT, GUI_APP_REMOVE_RATING);
	m_pRemoveRating->SetImageId(GUI_THEME_SALLY_ICON_RATING_OFF);
	m_pRemoveRating->SetText("Remove Rating");
	m_pRemoveRating->Visible(false);
	m_pFormView->AddChild(m_pRemoveRating);

	for (int i = 0; i < 7; i++)
	{
		int iX = 20 + 230 + 100;
		if ((i - 1) % 2)
			iX = 20;

		int iY = (i / 2 * 30) + 160;


		m_pDescriptionLabel[i] = new SallyAPI::GUI::CLabel(m_pFormView, iX, iY, 100);
		m_pDescriptionLabel[i]->SetFont("label.font.small");
		m_pDescriptionLabel[i]->SetBold(false);
		m_pDescriptionLabel[i]->SetAlign(DT_RIGHT | DT_VCENTER);
		m_pFormView->AddChild(m_pDescriptionLabel[i]);

		m_pStatusLabel[i] = new SallyAPI::GUI::CLabel(m_pFormView, iX + 100, iY, 220);
		m_pStatusLabel[i]->SetLocalised(false);
		m_pStatusLabel[i]->SetBold(false);
		m_pFormView->AddChild(m_pStatusLabel[i]);
	}

	m_pStatusLabel[6]->Resize(560, CONTROL_HEIGHT);

	m_pDescriptionLabel[0]->SetText("Track:");
	m_pDescriptionLabel[1]->SetText("Genre:");
	m_pDescriptionLabel[2]->SetText("Year:");
	m_pDescriptionLabel[3]->SetText("Composer:");
	m_pDescriptionLabel[4]->SetText("BitRate:");
	m_pDescriptionLabel[5]->SetText("Time Played:");
	m_pDescriptionLabel[6]->SetText("Filename:");

	m_pButtonEdit = new SallyAPI::GUI::CButton(m_pFormView, m_pFormView->GetWidth() - 150 - 20,
		m_pFormView->GetHeight() - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, GUI_EDIT_BUTTON);
	m_pButtonEdit->SetText("Edit");
	m_pButtonEdit->SetImageId(GUI_THEME_SALLY_ICON_EDIT);
	m_pFormView->AddChild(m_pButtonEdit);

	/***************************************/

	m_pFormEdit = new SallyAPI::GUI::CForm(this, m_pBackground->GetPositionX(), m_pBackground->GetPositionY(),
		m_pBackground->GetWidth(), m_pBackground->GetHeight());
	m_pFormEdit->Visible(false);
	this->AddChild(m_pFormEdit);

	m_pEditArtist = new SallyAPI::GUI::CEdit(m_pFormEdit, 55, 20, 700 - 55 - 20);
	m_pEditArtist->SetInfoText("Artist");
	m_pFormEdit->AddChild(m_pEditArtist);

	m_pEditTitle = new SallyAPI::GUI::CEdit(m_pFormEdit, 20, 60, 320);
	m_pEditTitle->SetInfoText("Title");
	m_pFormEdit->AddChild(m_pEditTitle);

	m_pEditAlbum = new SallyAPI::GUI::CEdit(m_pFormEdit, 20, 100, 320);
	m_pEditAlbum->SetInfoText("Album");
	m_pFormEdit->AddChild(m_pEditAlbum);

	m_pLabelBand = new SallyAPI::GUI::CLabel(m_pFormEdit, 20 + 320 + 10, 100, 100);
	m_pLabelBand->SetFont("label.font.small");
	m_pLabelBand->SetBold(false);
	m_pLabelBand->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pLabelBand->SetText("Album Interpret:");
	m_pFormEdit->AddChild(m_pLabelBand);

	m_pEditBand = new SallyAPI::GUI::CEdit(m_pFormEdit, 20 + 320 + 10 + 100, 100, 220);
	m_pEditBand->SetInfoText("Album Interpret");
	m_pFormEdit->AddChild(m_pEditBand);

	for (int i = 0; i < 4; i++)
	{
		int iX = 20 + 230 + 100;
		if ((i - 1) % 2)
			iX = 20;

		int iY = (i / 2 * 30) + 160;


		m_pEditDescriptionLabel[i] = new SallyAPI::GUI::CLabel(m_pFormEdit, iX, iY, 100);
		m_pEditDescriptionLabel[i]->SetFont("label.font.small");
		m_pEditDescriptionLabel[i]->SetBold(false);
		m_pEditDescriptionLabel[i]->SetAlign(DT_RIGHT | DT_VCENTER);
		m_pFormEdit->AddChild(m_pEditDescriptionLabel[i]);

		m_pEditStatusLabel[i] = new SallyAPI::GUI::CEdit(m_pFormEdit, iX + 100, iY, 220);
		m_pFormEdit->AddChild(m_pEditStatusLabel[i]);
	}

	m_pEditDescriptionLabel[0]->SetText("Track:");
	m_pEditDescriptionLabel[1]->SetText("Genre:");
	m_pEditDescriptionLabel[2]->SetText("Year:");
	m_pEditDescriptionLabel[3]->SetText("Composer:");

	m_pButtonSave = new SallyAPI::GUI::CButton(m_pFormEdit, m_pFormView->GetWidth() - 150 - 20,
		m_pFormView->GetHeight() - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, GUI_SAVE_BUTTON);
	m_pButtonSave->SetText("Save");
	m_pButtonSave->SetDefaultButton(true);
	m_pButtonSave->SetImageId(GUI_THEME_SALLY_ICON_SAVE);
	m_pFormEdit->AddChild(m_pButtonSave);

	m_pButtonCancel = new SallyAPI::GUI::CButton(m_pFormEdit, 20,
		m_pFormView->GetHeight() - CONTROL_HEIGHT - 20, 150, CONTROL_HEIGHT, GUI_CANCEL_BUTTON);
	m_pButtonCancel->SetText("Cancel");
	m_pButtonCancel->SetImageId(GUI_THEME_SALLY_ICON_CANCEL);
	m_pFormEdit->AddChild(m_pButtonCancel);
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
		case GUI_EDIT_BUTTON:
			if (!LoadFile(m_pStatusLabel[6]->GetText()))
				return;
			m_pFormView->Visible(false);
			m_pFormEdit->Visible(true);
			return;
		case GUI_SAVE_BUTTON:
			SaveFile();
		case GUI_CANCEL_BUTTON:
		case GUI_THEME_SALLY_CLOSE:
			m_pFormView->Visible(true);
			m_pFormEdit->Visible(false);
			break;
		}
		break;
	case GUI_RATING_CHANGED:
		OnCommandRatingChanged();
		break;

	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

bool CInfoPopUp::LoadFile(const std::string& filename)
{
	m_strFilename = filename;

	MP3FileInfo info;
	if (!info.Init(m_strFilename))
		return false;

	m_pEditArtist->SetText(info.GetSzArtist());
	m_pEditTitle->SetText(info.GetSzTitle());
	m_pEditAlbum->SetText(info.GetSzAlbum());
	m_pEditBand->SetText(info.GetSzBand());

	m_pEditStatusLabel[0]->SetText(info.GetSzTrack());
	m_pEditStatusLabel[1]->SetText(CAudioFile::GetMp3Genre(info.GetSzGenre()));
	m_pEditStatusLabel[2]->SetText(info.GetSzYear());
	m_pEditStatusLabel[3]->SetText(info.GetSzComposer());

	return true;
}

bool CInfoPopUp::SaveFile()
{
	MP3FileInfo info;
	if (!info.Init(m_strFilename))
		return false;

	info.SetSzArtist(m_pEditArtist->GetText());
	info.SetSzTitle(m_pEditTitle->GetText());
	info.SetSzAlbum(m_pEditAlbum->GetText());
	info.SetSzBand(m_pEditBand->GetText());

	info.SetSzTrack(m_pEditStatusLabel[0]->GetText());
	info.SetSzGenre(m_pEditStatusLabel[1]->GetText());
	info.SetSzYear(m_pEditStatusLabel[2]->GetText());
	info.SetSzComposer(m_pEditStatusLabel[3]->GetText());

	info.Update();

	m_pParent->SendMessageToParent(this, 0, GUI_APP_RELOAD_MP3_INFO);

	return true;
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

void CInfoPopUp::UpdateInfo(CMediaPlayer* mediaPlayer, const std::string& timeplayed, int rating)
{
	if (mediaPlayer->GetType() == MEDIAFILE_AUDIO)
	{
		MP3FileInfo* id3Tag = mediaPlayer->GetMp3Tag();

		if (id3Tag != NULL)
		{
			std::string strAlbum = id3Tag->GetSzAlbum();

			if (id3Tag->GetSzBand().length() > 0)
			{
				strAlbum.append(" (");
				strAlbum.append(id3Tag->GetSzBand());
				strAlbum.append(")");
			}

			m_pArtist->SetText(id3Tag->GetSzArtist());
			m_pTitle->SetText(id3Tag->GetSzTitle());
			m_pAlbum->SetText(strAlbum);
			m_pStatusLabel[0]->SetText(id3Tag->GetSzTrack());
			m_pStatusLabel[1]->SetText(CAudioFile::GetMp3Genre(id3Tag->GetSzGenre()));
			m_pStatusLabel[2]->SetText(id3Tag->GetSzYear());
			m_pStatusLabel[3]->SetText(id3Tag->GetSzComposer());
			m_pStatusLabel[4]->SetText(id3Tag->GetSzBitRate());
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

		mediaPlayer->UnlockMedia();
	}
	else if (mediaPlayer->GetType() == MEDIAFILE_VIDEO)
	{
		m_pArtist->SetText(SallyAPI::String::PathHelper::GetFileFromPath(mediaPlayer->GetFilename()));
		m_pAlbum->SetText("");
		m_pTitle->SetText("");
		m_pStatusLabel[0]->SetText("");
		m_pStatusLabel[1]->SetText("");
		m_pStatusLabel[2]->SetText("");
		m_pStatusLabel[3]->SetText("");
		m_pStatusLabel[4]->SetText("");
	}

	m_pStatusLabel[5]->SetText(timeplayed);
	m_pStatusLabel[6]->SetText(mediaPlayer->GetFilename());

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