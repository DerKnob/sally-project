////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	PaintBoard\AppPaintBoard.cpp
///
/// \brief	Implements the application paint board class. 
///
/// \author	Christian Knobloch
/// \date	13.09.2010
///
/// This file is part of the Sally Project
/// 
/// Copyright(c) 2008-2010 Sally Project
/// http://www.sally-project.de/
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

#include "AppPaintBoard.h"

CAppPaintBoard::CAppPaintBoard(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CApplicationWindow(parent, graphicId, pluginPath)
{
	LoadApplicationImage("color_white.png", GUI_APP_COLOR_WHITE);
	LoadApplicationImage("color_black.png", GUI_APP_COLOR_BLACK);
	LoadApplicationImage("color_blue.png", GUI_APP_COLOR_BLUE);
	LoadApplicationImage("color_green.png", GUI_APP_COLOR_GREEN);
	LoadApplicationImage("color_red.png", GUI_APP_COLOR_RED);

	LoadApplicationImage("draw_brush.png", GUI_APP_DRAW_BRUSH);
	LoadApplicationImage("draw_eraser.png", GUI_APP_DRAW_ERASER);

	// Create GUI
	m_pSideMenu = new SallyAPI::GUI::CSideMenu(this);
	this->AddChild(m_pSideMenu);

	m_pSideMenuEmpty = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL, GUI_APP_CLEAR_PAINT_BOARD);
	m_pSideMenuEmpty->SetImageId(GUI_THEME_SALLY_KEYBOARD_CLEAR);
	m_pSideMenuEmpty->SetText("Clear");
	m_pSideMenu->AddChild(m_pSideMenuEmpty);

	m_pSideMenuSeperator1 = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_SEPERATOR);
	m_pSideMenu->AddChild(m_pSideMenuSeperator1);

	/************************************************************************/
	/* Eraser                                                               */
	/************************************************************************/
	m_pSideMenuDrawEraser = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL, GUI_APP_DRAW_ERASER);
	m_pSideMenuDrawEraser->SetImageId(GUI_APP_DRAW_ERASER + GetGraphicId());
	m_pSideMenuDrawEraser->SetText("Eraser");
	m_pSideMenu->AddChild(m_pSideMenuDrawEraser);

	/************************************************************************/
	/* Brush                                                                */
	/************************************************************************/
	m_pSideMenuDrawBrush = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_NORMAL, GUI_APP_DRAW_BRUSH);
	m_pSideMenuDrawBrush->SetImageId(GUI_APP_DRAW_BRUSH + GetGraphicId());
	m_pSideMenuDrawBrush->SetText("Brush");
	m_pSideMenu->AddChild(m_pSideMenuDrawBrush);

	m_pSideMenuSeperator2 = new SallyAPI::GUI::CSideMenuButton(m_pSideMenu,
		SallyAPI::GUI::SIDE_MENUE_BUTTON_TYPE_SEPERATOR);
	m_pSideMenu->AddChild(m_pSideMenuSeperator2);

	// Colors
	m_pSideMenuColor = new SallyAPI::GUI::CDropDown(m_pSideMenu, 0, 0, 0);
	m_pSideMenuColor->SetLocalised(true);
	m_pSideMenu->AddChild(m_pSideMenuColor);

	m_pSideMenuColor->Move(10, m_pSideMenuColor->GetPositionY());
	m_pSideMenuColor->Resize(200 - 20, CONTROL_HEIGHT);

	SallyAPI::GUI::CDropDownItem itemWhite("white", "White", GUI_APP_COLOR_WHITE + GetGraphicId());
	m_pSideMenuColor->AddItem(itemWhite);

	SallyAPI::GUI::CDropDownItem itemBlack("black", "Black", GUI_APP_COLOR_BLACK + GetGraphicId());
	m_pSideMenuColor->AddItem(itemBlack);

	SallyAPI::GUI::CDropDownItem itemBlue("blue", "Blue", GUI_APP_COLOR_BLUE + GetGraphicId());
	m_pSideMenuColor->AddItem(itemBlue);

	SallyAPI::GUI::CDropDownItem itemRed("red", "Red", GUI_APP_COLOR_RED + GetGraphicId());
	m_pSideMenuColor->AddItem(itemRed);

	SallyAPI::GUI::CDropDownItem itemGreen("green", "Green", GUI_APP_COLOR_GREEN + GetGraphicId());
	m_pSideMenuColor->AddItem(itemGreen);

	// Slider
	m_pSideMenuPenSizeLabel = new SallyAPI::GUI::CLabel(m_pSideMenu, 0, 0, 0);
	m_pSideMenuPenSizeLabel->SetLocalised(false);
	m_pSideMenuPenSizeLabel->SetFont("sidebutton.font");
	m_pSideMenu->AddChild(m_pSideMenuPenSizeLabel);

	m_pSideMenuPenSizeLabel->Move(10, m_pSideMenuPenSizeLabel->GetPositionY());
	m_pSideMenuPenSizeLabel->Resize(200 - 20, CONTROL_HEIGHT);

	m_pSideMenuPenSize = new SallyAPI::GUI::CSlider(m_pSideMenu, 0, 0, 0);
	m_pSideMenuPenSize->SetMaxPosition(19);
	m_pSideMenu->AddChild(m_pSideMenuPenSize);

	m_pSideMenuPenSize->Move(10, m_pSideMenuPenSize->GetPositionY());
	m_pSideMenuPenSize->Resize(200 - 20, CONTROL_HEIGHT);

	m_pPaintArea = new CPaintArea(this, 0, 0, WINDOW_WIDTH - MENU_WIDTH, WINDOW_HEIGHT);
	this->AddChild(m_pPaintArea);
}

CAppPaintBoard::~CAppPaintBoard()
{
}

void CAppPaintBoard::SaveConfig()
{
	m_pPaintArea->SaveConfig();
}

void CAppPaintBoard::LoadConfig()
{
	std::string color = GetPropertyString("color", "black");
	m_pSideMenuColor->SelectItemByIdentifier(color);

	int penSize = GetPropertyInt("penSize", 5);
	m_pSideMenuPenSize->SetPosition(penSize);

	UpdatePenSizeInfo(penSize);

	int drawTool = GetPropertyInt("drawTool", 0);
	if (drawTool == (int) DRAW_ERASER)
	{
		m_pPaintArea->SetDrawTool(DRAW_ERASER);
		m_pSideMenuDrawBrush->SetCheckStatus(false);
		m_pSideMenuDrawEraser->SetCheckStatus(true);
		m_pSideMenuColor->Enable(false);
	}
	else
	{
		m_pPaintArea->SetDrawTool(DRAW_BRUSH);
		m_pSideMenuDrawBrush->SetCheckStatus(true);
		m_pSideMenuDrawEraser->SetCheckStatus(false);
		m_pSideMenuColor->Enable(true);
	}
}

void CAppPaintBoard::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_SLIDER_MOVED:
	case GUI_SLIDER_CLICKED:
		SetPropertyInt("penSize", m_pSideMenuPenSize->GetPosition());

		UpdatePenSizeInfo(m_pSideMenuPenSize->GetPosition());
		return;
	case MS_DIALOG_YES:
		m_pPaintArea->ClearPaintArea();
		return;
	case GUI_DROPDOWN_CHANGED:
		SetPropertyString("color", m_pSideMenuColor->GetSelectedIdentifier());
		return;
	case GUI_BUTTON_CLICKED:
		switch (reporterId)
		{
		case GUI_APP_CLEAR_PAINT_BOARD:
			OnCommandClearPaintBoard();
			return;
		case GUI_APP_DRAW_BRUSH:
			m_pSideMenuDrawBrush->SetCheckStatus(true);
			m_pSideMenuDrawEraser->SetCheckStatus(false);
			m_pSideMenuColor->Enable(true);
			m_pPaintArea->SetDrawTool(DRAW_BRUSH);
			return;
		case GUI_APP_DRAW_ERASER:
			m_pSideMenuDrawBrush->SetCheckStatus(false);
			m_pSideMenuDrawEraser->SetCheckStatus(true);
			m_pSideMenuColor->Enable(false);
			m_pPaintArea->SetDrawTool(DRAW_ERASER);
			return;
		}
	}
	SallyAPI::GUI::CApplicationWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

void CAppPaintBoard::UpdatePenSizeInfo(int penSize)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CLanguageManager* lang = config->GetLanguageLocalization();
	std::string info;

	info.append(lang->GetString("Brush Size: %s", SallyAPI::String::StringHelper::ConvertToString(penSize + 1).c_str(), NULL));
	m_pSideMenuPenSizeLabel->SetText(info);
}

void CAppPaintBoard::OnCommandClearPaintBoard()
{
	SallyAPI::GUI::SendMessage::CParameterQuestionBox questionBox(GUI_APP_CLEAR_PAINT_BOARD, this, "Do you really want do clear the board?");
	m_pParent->SendMessageToParent(this, m_iControlId, MS_SALLY_SHOW_QUESTIONBOX, &questionBox);
}

void CAppPaintBoard::SallyConfigChanged()
{
	int penSize = GetPropertyInt("penSize", 5);
	UpdatePenSizeInfo(penSize);
}