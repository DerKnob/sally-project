////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	Community\AppCommunity.h
///
/// \brief	Declares the application community class. 
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
#include <sallyAPI\sallyAPI.h>
#include <xmlParser.h>

#include "Define.h"
#include "ControlGroup.h"

#pragma comment(lib, "xmlparser.lib")

class CAppCommunity :
	public SallyAPI::GUI::CApplicationWindow
{
private:
	SallyAPI::GUI::CImageBox*				m_pImageHeader;
	SallyAPI::GUI::CImageBox*				m_pImageLogo;

	SallyAPI::GUI::CImageBox*				m_pApplicationImage;
	SallyAPI::GUI::CLabel*					m_pWelcome;

	SallyAPI::GUI::CTabcontrol*				m_pTabControl;
	SallyAPI::GUI::CTabcontrolItem*			m_pTabHome;
	SallyAPI::GUI::CScrollForm*				m_pTabHomeForm;
	SallyAPI::GUI::CTabcontrolItem*			m_pTabNews;
	SallyAPI::GUI::CScrollForm*				m_pTabNewsForm;
	SallyAPI::GUI::CTabcontrolItem*			m_pTabWall;
	SallyAPI::GUI::CScrollForm*				m_pTabWallForm;

	SallyAPI::GUI::CButton*					m_pUpdateStatus;
	SallyAPI::GUI::CEdit*					m_pUpdateStatusEdit;

	std::vector<CControlGroup*>				m_vControlGroupHome;
	std::vector<CControlGroup*>				m_vControlGroupWall;
	std::vector<CControlGroup*>				m_vControlGroupNews;
	int										m_iShowCount;
	int										m_iShowRows;
	int										m_iShowCols;

	SallyAPI::GUI::CPicture*	LoadImage(std::map<std::string, SallyAPI::GUI::CPicture*>& m_pPicturesNew, 
										  const std::string& userId);

	bool	UpdateFacebookWall();
	bool	UpdateFacebookNews();
	bool	UpdateFacebookSally();
	void	UpdateFacebookStatus();
	
	void	OnCommandUpdateView();
	void	OnCommandUpdateStatus();
public:
	CAppCommunity(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CAppCommunity();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void Visible(bool visible);
	virtual bool IsFacebookNeeded();
};
