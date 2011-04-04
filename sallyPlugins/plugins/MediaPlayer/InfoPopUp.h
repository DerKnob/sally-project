////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	MediaPlayer\InfoPopUp.h
///
/// \brief	Declares the information pop up class. 
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

#pragma once
#include "Define.h"
#include <sallyAPI\sallyAPI.h>
#include "AudioFile.h"
#include "VideoFile.h"

class CInfoPopUp
	: public SallyAPI::GUI::CPopUpWindow
{
private:
	std::string					m_strFilename;

	SallyAPI::GUI::CForm*		m_pFormView;
	SallyAPI::GUI::CButton*		m_pRemoveRating;
	SallyAPI::GUI::CLabel*		m_pRatingDescription;

	SallyAPI::GUI::CGroupBox*	m_pBackground;

	SallyAPI::GUI::CImageBox*	m_pIcon;
	SallyAPI::GUI::CLabel*		m_pArtist;
	SallyAPI::GUI::CLabel*		m_pTitle;
	SallyAPI::GUI::CLabel*		m_pAlbum;
	SallyAPI::GUI::CLabel*		m_pDescriptionLabel[7];
	SallyAPI::GUI::CLabel*		m_pStatusLabel[7];
	SallyAPI::GUI::CRating*		m_pRating;
	SallyAPI::GUI::CButton*		m_pButtonEdit;


	SallyAPI::GUI::CForm*		m_pFormEdit;
	SallyAPI::GUI::CEdit*		m_pEditArtist;
	SallyAPI::GUI::CEdit*		m_pEditTitle;
	SallyAPI::GUI::CEdit*		m_pEditAlbum;
	SallyAPI::GUI::CEdit*		m_pEditBand;
	SallyAPI::GUI::CLabel*		m_pLabelBand;
	SallyAPI::GUI::CLabel*		m_pEditDescriptionLabel[4];
	SallyAPI::GUI::CEdit*		m_pEditStatusLabel[4];

	SallyAPI::GUI::CButton*		m_pButtonSave;
	SallyAPI::GUI::CButton*		m_pButtonCancel;

	bool	LoadFile(const std::string& filename);
	bool	SaveFile();

	void	OnCommandRatingChanged();
	void	OnCommandRemoveRating();
public:
	CInfoPopUp(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& explicidAppName);
	virtual ~CInfoPopUp();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);

	void UpdateMp3(CAudioFile* mp3File, const std::string& timeplayed, int rating);
	void UpdateVideo(CVideoFile* currentFile, const std::string& timeplayed, int rating);
	void UpdateRating(int rating);
};
