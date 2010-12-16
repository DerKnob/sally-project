////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	PaintBoard\AppPaintBoard.h
///
/// \brief	Declares the application paint board class. 
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
#include "Define.h"
#include "PaintArea.h"

class CAppPaintBoard :
	public SallyAPI::GUI::CApplicationWindow
{
private:
	SallyAPI::GUI::CSideMenu*			m_pSideMenu;

	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuEmpty;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuSeperator1;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuDrawEraser;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuDrawBrush;
	SallyAPI::GUI::CSideMenuButton*		m_pSideMenuSeperator2;
	SallyAPI::GUI::CDropDown*			m_pSideMenuColor;
	SallyAPI::GUI::CLabel*				m_pSideMenuPenSizeLabel;
	SallyAPI::GUI::CSlider*				m_pSideMenuPenSize;

	CPaintArea*							m_pPaintArea;

	void	OnCommandClearPaintBoard();
	void	UpdatePenSizeInfo(int penSize);

	virtual void	LoadConfig();
	virtual void	SaveConfig();
	virtual void	SallyConfigChanged();
public:
	CAppPaintBoard(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CAppPaintBoard();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int iMessageID, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
