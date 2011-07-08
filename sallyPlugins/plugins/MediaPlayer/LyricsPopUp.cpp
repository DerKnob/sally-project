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

	m_pBackground = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 500) / 2, (WINDOW_HEIGHT - 550) / 2, 500, 550);
	m_pBackground->SetAlwaysHandleInput(true);
	this->AddChild(m_pBackground);

	m_pIcon = new SallyAPI::GUI::CImageBox(m_pBackground, -20, -20, 64, 64);
	m_pIcon->SetImageId(GetGraphicId());
	m_pBackground->AddChild(m_pIcon);

	m_pTitle = new SallyAPI::GUI::CLabel(m_pBackground, 55, 20, 500 - 55 - 20);
	m_pTitle->SetFont("big2.font");
	m_pTitle->SetLocalised(false);
	m_pBackground->AddChild(m_pTitle);

	m_pArtist = new SallyAPI::GUI::CLabel(m_pBackground, 55, 50, 500 - 55 - 20);
	m_pArtist->SetLocalised(false);
	m_pBackground->AddChild(m_pArtist);

	m_pText = new SallyAPI::GUI::CLabelBox(m_pBackground, 20, 90, 500 - 20 - 20, 550 - 90 - 20, true);
	m_pText->SetLocalised(false);
	m_pText->SetAlign(DT_CENTER | DT_WORDBREAK);
	m_pBackground->AddChild(m_pText);

	m_LyricGetter.SetStaticValues((dynamic_cast<SallyAPI::GUI::CAppBase*> (m_pParent)), m_pText);
}

CLyricsPopUp::~CLyricsPopUp()
{
}

void CLyricsPopUp::GetLyric(const std::string& artist, const std::string& title)
{
	std::string temp = "by ";
	temp.append(artist);

	m_pArtist->SetText(temp);

	m_pTitle->SetText(title);

	m_pText->SetText("");

	m_LyricGetter.SetValues(artist, title);
	m_LyricGetter.Start();
}
