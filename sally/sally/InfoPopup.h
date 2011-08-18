////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\InfoPopup.h
///
/// \brief	Declares the information popup class. 
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

class CInfoPopup
	: public SallyAPI::GUI::CPopUpWindow
{
private:
	int								m_pIdCounter;

	SallyAPI::GUI::CGroupBox*		m_pBackgroundGroupBox;
	SallyAPI::GUI::CImageBox*		m_pIcon;
	SallyAPI::GUI::CLabel*			m_pText;
	SallyAPI::GUI::CLabelBox*		m_pDescription;

	std::vector<SallyAPI::GUI::SendMessage::CParameterInfoPopup>	m_vParameterInfoList;
	SallyAPI::GUI::CTimer*			m_tBlendOutTimer;
	SallyAPI::System::CCritSection	m_Lock;

	void	ResizeGroupBox();
	void	ShowNext();
	void	CalculateIconPosition(SallyAPI::GUI::CPicture* picture, int i);
	void	OnCommandBlendOut();
	void	OnCommandBlended();
public:
	CInfoPopup(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CInfoPopup();

	int		AddItem(SallyAPI::GUI::SendMessage::CParameterInfoPopup& parameter);
	void	RemoveItem(int id);

	virtual void Render();
	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void SendMessageToChilds(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
