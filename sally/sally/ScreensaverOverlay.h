////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\ScreensaverOverlay.h
///
/// \brief	Declares the screensaver overlay class. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2011 Sally Project
/// http://www.sally-project.org/
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU Lesser General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
/// 
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
/// GNU Lesser General Public License for more details.
/// 
/// You should have received a copy of the GNU Lesser General Public License
/// along with this program. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Define.h"

class CScreensaverOverlay
	: public SallyAPI::GUI::CApplicationWindow
{
private:
	SallyAPI::GUI::CTopMenu*							m_pTopMenu;
	SallyAPI::GUI::CVolumeControl*						m_pVolumeControl;
	SallyAPI::GUI::CButton*								m_pButtonCloseFullscreen;
	std::vector<SallyAPI::GUI::CScreensaverControls*>	m_pScreensaverControlsList;
	SallyAPI::GUI::CScreensaverControls*				m_pCurrentControls;

	void	OnCommandHideMenu();
	void	OnCommandShowMenu(SallyAPI::GUI::CGUIBaseObject* reporter);
public:
	CScreensaverOverlay(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CScreensaverOverlay();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void Visible(bool visible);

	void	AddScreensaverControl(SallyAPI::GUI::CGUIBaseObject* control);
};
