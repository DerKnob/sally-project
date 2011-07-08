////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Keyboard.cpp
///
/// \brief	Implements the keyboard class. 
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

#include "Keyboard.h"

using namespace SallyAPI::GUI;

#define GUI_KEYBOARD_CONTROL1					100
#define GUI_KEYBOARD_CONTROL2					200
#define GUI_KEYBOARD_CONTROL3					300
#define GUI_KEYBOARD_CONTROL4					400
#define GUI_KEYBOARD_CONTROLNUMBER				500
#define GUI_KEYBOARD_LABEL1						500
#define GUI_KEYBOARD_CONTROL_ENTER				2000
#define GUI_KEYBOARD_CONTROL_DELETE				2001
#define GUI_KEYBOARD_CONTROL_SPACE				2002
#define GUI_KEYBOARD_CONTROL_UMSCHALT			2003
#define GUI_KEYBOARD_CONTROL_ALT				2004
#define GUI_KEYBOARD_CONTROL_CLEAR				2005
#define GUI_KEYBOARD_CONTROL_UMSCHALT_PRESSED	2006
#define GUI_KEYBOARD_CONTROL_TAB				2007
#define GUI_KEYBOARD_CONTROLNUMBERPLUS			2008

CKeyboard::CKeyboard(SallyAPI::GUI::CGUIBaseObject* parent)
	:SallyAPI::GUI::CPopUpWindow(parent, 0, ""), m_iSmartKeyboardUsed(0)
{
	m_pSmartKeyboardGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 790) / 2, WINDOW_HEIGHT - 540, 790, 200);
	m_pSmartKeyboardGroupBox->Visible(false);
	this->AddChild(m_pSmartKeyboardGroupBox);

	m_pKeyboardGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 630) / 2, WINDOW_HEIGHT  - 310, 630, 290);
	m_pKeyboardGroupBox->Visible(false);
	this->AddChild(m_pKeyboardGroupBox);

	m_pNumberPadGroupBox = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 240) / 2, WINDOW_HEIGHT  - 360, 240, 330);
	m_pNumberPadGroupBox->Visible(false);
	this->AddChild(m_pNumberPadGroupBox);
	
	// smart keyboard
	for (int k = 0; k < SMART_KEYBOARD_ROW_COUNT; ++k)
	{
		m_pSmartKeyboardLabel[k] = new SallyAPI::GUI::CLabel(m_pSmartKeyboardGroupBox,
			10, 10 + (CONTROL_HEIGHT * k), 250,
			GUI_KEYBOARD_LABEL1 + k);
		m_pSmartKeyboardLabel[k]->SetLocalised(false);
		m_pSmartKeyboardGroupBox->AddChild(m_pSmartKeyboardLabel[k]);
	}
	for (int k = 0; k < SMART_KEYBOARD_ROW_COUNT; ++k)
	{
		m_pSmartKeyboardLabel[k + SMART_KEYBOARD_ROW_COUNT] = new SallyAPI::GUI::CLabel(m_pSmartKeyboardGroupBox,
			10 + 250 + 10,  10 + (CONTROL_HEIGHT * k), 250,
			GUI_KEYBOARD_LABEL1 + k);
		m_pSmartKeyboardLabel[k + SMART_KEYBOARD_ROW_COUNT]->SetLocalised(false);
		m_pSmartKeyboardGroupBox->AddChild(m_pSmartKeyboardLabel[k + SMART_KEYBOARD_ROW_COUNT]);
	}
	for (int k = 0; k < SMART_KEYBOARD_ROW_COUNT; ++k)
	{
		m_pSmartKeyboardLabel[k + (SMART_KEYBOARD_ROW_COUNT * 2)] = new SallyAPI::GUI::CLabel(m_pSmartKeyboardGroupBox,
			10 + 250 + 10 + 250 + 10,  10 + (CONTROL_HEIGHT * k), 250,
			GUI_KEYBOARD_LABEL1 + k);
		m_pSmartKeyboardLabel[k + (SMART_KEYBOARD_ROW_COUNT * 2)]->SetLocalised(false);
		m_pSmartKeyboardGroupBox->AddChild(m_pSmartKeyboardLabel[k + (SMART_KEYBOARD_ROW_COUNT * 2)]);
	}

	// number keyboard
	int i = 0;
	for (int k = 0; k < 3; ++k)
	{
		for (int l = 0; l < 3; ++l)
		{
			int x = (CONTROL_HEIGHT + 40) * l + 20;
			int y = (CONTROL_HEIGHT + 20) * k + 70;
			m_pButtonNumber[i] = new SallyAPI::GUI::CButton(m_pNumberPadGroupBox,
				x, y, CONTROL_HEIGHT + 30, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROLNUMBER + i);
			m_pButtonNumber[i]->SetLocalised(false);
			m_pButtonNumber[i]->UseHoleWidth(true);
			m_pButtonNumber[i]->SetText(SallyAPI::String::StringHelper::ConvertToString(i + 1));
			m_pButtonNumber[i]->SetAlign(DT_CENTER | DT_VCENTER);
			m_pNumberPadGroupBox->AddChild(m_pButtonNumber[i]);

			++i;
		}
	}
	m_pButtonNumber[9] = new SallyAPI::GUI::CButton(m_pNumberPadGroupBox,
		20, ((CONTROL_HEIGHT + 10) * 3) + 70 + 30, CONTROL_HEIGHT + 30, CONTROL_HEIGHT + 10,
		GUI_KEYBOARD_CONTROLNUMBERPLUS);
	m_pButtonNumber[9]->SetLocalised(false);
	m_pButtonNumber[9]->UseHoleWidth(true);
	m_pButtonNumber[9]->SetText("-/+");
	m_pButtonNumber[9]->SetAlign(DT_CENTER | DT_VCENTER);
	m_pNumberPadGroupBox->AddChild(m_pButtonNumber[9]);

	m_pButtonNumber[10] = new SallyAPI::GUI::CButton(m_pNumberPadGroupBox,
		20 + 60 + 10, ((CONTROL_HEIGHT + 10) * 3) + 70 + 30, CONTROL_HEIGHT + 30, CONTROL_HEIGHT + 10,
		GUI_KEYBOARD_CONTROLNUMBER + 9);
	m_pButtonNumber[10]->SetLocalised(false);
	m_pButtonNumber[10]->UseHoleWidth(true);
	m_pButtonNumber[10]->SetText("0");
	m_pButtonNumber[10]->SetAlign(DT_CENTER | DT_VCENTER);
	m_pNumberPadGroupBox->AddChild(m_pButtonNumber[10]);

	m_pButtonNumber[11] = new SallyAPI::GUI::CButton(m_pNumberPadGroupBox,
		20 + 60 + 10 + 60 + 10, ((CONTROL_HEIGHT + 10) * 3) + 70 + 30, CONTROL_HEIGHT + 30, CONTROL_HEIGHT + 10,
		 GUI_KEYBOARD_CONTROL_CLEAR, BUTTON_TYPE_NORMAL);
	m_pButtonNumber[11]->SetLocalised(false);
	m_pButtonNumber[11]->UseHoleWidth(true);
	m_pButtonNumber[11]->SetImageId(GUI_THEME_SALLY_KEYBOARD_CLEAR);
	m_pNumberPadGroupBox->AddChild(m_pButtonNumber[11]);

	m_pButtonNumber[12] = new SallyAPI::GUI::CButton(m_pNumberPadGroupBox,
		20, ((CONTROL_HEIGHT + 10) * 4) + 70 + 40, 200, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_ENTER, BUTTON_TYPE_NORMAL);
	m_pButtonNumber[12]->SetLocalised(false);
	m_pButtonNumber[12]->UseHoleWidth(true);
	m_pButtonNumber[12]->SetDefaultButton(true);
	m_pButtonNumber[12]->SetText("Enter");
	m_pButtonNumber[12]->SetAlign(DT_CENTER | DT_VCENTER);
	m_pNumberPadGroupBox->AddChild(m_pButtonNumber[12]);

	m_pButtonNumber[13] = new SallyAPI::GUI::CButton(m_pNumberPadGroupBox,
		240 - 20 - 60, 20, CONTROL_HEIGHT + 30, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_DELETE, BUTTON_TYPE_NORMAL);
	m_pButtonNumber[13]->SetLocalised(false);
	m_pButtonNumber[13]->UseHoleWidth(true);
	m_pButtonNumber[13]->SetImageId(GUI_THEME_SALLY_KEYBOARD_BACKSPACE);
	m_pNumberPadGroupBox->AddChild(m_pButtonNumber[13]);

	// complete keyboard
	int l = 0;
	for (int k = 0; k < 13; ++k)
	{
		m_pButton1[k] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + (k * (CONTROL_HEIGHT + 10)), 70, CONTROL_HEIGHT + 10,
			CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL1 + k, BUTTON_TYPE_FOUR_TEXTS);
		m_pButton1[k]->SetLocalised(false);
		m_pButton1[k]->UseHoleWidth(true);
		m_pButton1[k]->SetAlign(DT_CENTER | DT_VCENTER);
		m_pKeyboardGroupBox->AddChild(m_pButton1[k]);
	}

	for (int k = 0; k < 12; ++k)
	{
		m_pButton2[k] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 50 + (k * (CONTROL_HEIGHT + 10)), 110, CONTROL_HEIGHT + 10,
			CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL2 + k, BUTTON_TYPE_FOUR_TEXTS);
		m_pButton2[k]->SetLocalised(false);
		m_pButton2[k]->UseHoleWidth(true);
		m_pButton2[k]->SetAlign(DT_CENTER | DT_VCENTER);
		m_pKeyboardGroupBox->AddChild(m_pButton2[k]);
	}

	for (int k = 0; k < 12; ++k)
	{
		m_pButton3[k] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 80 + (k * (CONTROL_HEIGHT + 10)), 150, CONTROL_HEIGHT + 10,
			CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL3 + k, BUTTON_TYPE_FOUR_TEXTS);
		m_pButton3[k]->SetLocalised(false);
		m_pButton3[k]->UseHoleWidth(true);
		m_pButton3[k]->SetAlign(DT_CENTER | DT_VCENTER);
		m_pKeyboardGroupBox->AddChild(m_pButton3[k]);
	}

	for (int k = 0; k < 11; ++k)
	{
		m_pButton4[k] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 60 + (k * (CONTROL_HEIGHT + 10)), 190,
			CONTROL_HEIGHT + 10, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL4 + k, BUTTON_TYPE_FOUR_TEXTS);
		m_pButton4[k]->SetLocalised(false);
		m_pButton4[k]->UseHoleWidth(true);
		m_pButton4[k]->SetAlign(DT_CENTER | DT_VCENTER);
		m_pKeyboardGroupBox->AddChild(m_pButton4[k]);
	}

	m_pButton5[0] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20, 190, CONTROL_HEIGHT + 30, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_UMSCHALT);
	m_pButton5[0]->SetLocalised(false);
	m_pButton5[0]->UseHoleWidth(true);
	m_pButton5[0]->SetImageId(GUI_THEME_SALLY_KEYBOARD_CAPS);
	m_pKeyboardGroupBox->AddChild(m_pButton5[0]);

	m_pButton5[1] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 500 , 190, CONTROL_HEIGHT + 60, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_UMSCHALT);
	m_pButton5[1]->SetLocalised(false);
	m_pButton5[1]->UseHoleWidth(true);
	m_pButton5[1]->SetImageId(GUI_THEME_SALLY_KEYBOARD_CAPS);
	m_pKeyboardGroupBox->AddChild(m_pButton5[1]);

	m_pButton5[2] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20, 150, CONTROL_HEIGHT + 50, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_UMSCHALT_PRESSED);
	m_pButton5[2]->SetLocalised(false);
	m_pButton5[2]->UseHoleWidth(true);
	m_pButton5[2]->SetImageId(GUI_THEME_SALLY_KEYBOARD_CAPSLOCK);
	m_pKeyboardGroupBox->AddChild(m_pButton5[2]);

	m_pButton5[3] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 520, 70, CONTROL_HEIGHT + 40, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_DELETE);
	m_pButton5[3]->SetLocalised(false);
	m_pButton5[3]->UseHoleWidth(true);
	m_pButton5[3]->SetImageId(GUI_THEME_SALLY_KEYBOARD_BACKSPACE);
	m_pKeyboardGroupBox->AddChild(m_pButton5[3]);

	m_pButton5[4] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 20, 230, 475, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_SPACE);
	m_pButton5[4]->SetLocalised(false);
	m_pButton5[4]->UseHoleWidth(true);
	m_pKeyboardGroupBox->AddChild(m_pButton5[4]);

	m_pButton5[5] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 495, 230, CONTROL_HEIGHT + 40, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_ALT);
	m_pButton5[5]->SetLocalised(false);
	m_pButton5[5]->UseHoleWidth(true);
	m_pButton5[5]->SetText("Alt Gr");
	m_pButton5[5]->SetAlign(DT_CENTER | DT_VCENTER);
	m_pKeyboardGroupBox->AddChild(m_pButton5[5]);

	m_pButton5[6] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 530, 110, CONTROL_HEIGHT + 30, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_ENTER);
	m_pButton5[6]->SetLocalised(false);
	m_pButton5[6]->UseHoleWidth(true);
	m_pButton5[6]->SetDefaultButton(true);
	m_pButton5[6]->SetText("Enter");
	m_pButton5[6]->SetAlign(DT_CENTER | DT_VCENTER);
	m_pKeyboardGroupBox->AddChild(m_pButton5[6]);

	m_pButton5[7] = new SallyAPI::GUI::CButton(m_pKeyboardGroupBox, 20 + 550, 20, CONTROL_HEIGHT + 10, CONTROL_HEIGHT + 10, GUI_KEYBOARD_CONTROL_CLEAR);
	m_pButton5[7]->SetLocalised(false);
	m_pButton5[7]->UseHoleWidth(true);
	m_pButton5[7]->SetImageId(GUI_THEME_SALLY_KEYBOARD_CLEAR);
	m_pKeyboardGroupBox->AddChild(m_pButton5[7]);

	m_pEditInput = new SallyAPI::GUI::CEdit(this, 0, 0, 0);
	m_pEditInput->SetLocalised(false);
	m_pEditInput->SetKeyboardInputEnabled(false);
	this->AddChild(m_pEditInput);

	CheckDatabaseExists();
}

CKeyboard::~CKeyboard()
{
}

void CKeyboard::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	std::string text;
	std::string temp;

	switch (messageId)
	{
	case GUI_LABEL_CLICKED:
		if (reporterId >= GUI_KEYBOARD_LABEL1 && reporterId <= GUI_KEYBOARD_LABEL1 + 100)
		{
			OnCommandSmartKeyboard(reporter);
		}
		return;
	case GUI_BUTTON_CLICKED:
		if (reporterId >= GUI_KEYBOARD_CONTROL1 && reporterId <= GUI_KEYBOARD_CONTROLNUMBER + 100)
		{
			if (reporter->GetText().compare(" ") != 0)
			{
				if (CheckMaxLength())
				{
					text = m_pEditInput->GetText();

					text.append(reporter->GetText());
					m_pEditInput->SetText(text);
					if (m_pButton5[0]->GetCheckStatus())
					{
						m_pButton5[0]->SetCheckStatus(false);
						m_pButton5[1]->SetCheckStatus(false);
						SetButtonText(0);
					}
					if (m_pButton5[5]->GetCheckStatus())
					{
						m_pButton5[5]->SetCheckStatus(false);
						SetButtonText(0);
					}
				}
			}
		}
		else
		{
			switch (reporterId)
			{
			case GUI_KEYBOARD_CONTROLNUMBERPLUS:
				OnCommandChangeSign();
				return;
			case GUI_KEYBOARD_CONTROL_CLEAR:
				m_pEditInput->SetText("");
				HideSmartKeyboard();
				return;
			case GUI_KEYBOARD_CONTROL_ENTER:
				m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_KEYBOARD);
				
				// save value if it is not a password
				if ((!m_bPassword) && (!m_bNumberOnly))
				{
					m_tKeyboardBrain.SetValues(m_pEditInput->GetText());
					m_tKeyboardBrain.Start();
				}
				return;
			case GUI_KEYBOARD_CONTROL_SPACE:
				if (CheckMaxLength())
				{
					text = m_pEditInput->GetText();

					text.append(" ");
					m_pEditInput->SetText(text);
				}
				return;
			case GUI_KEYBOARD_CONTROL_UMSCHALT:
				if (m_pButton5[0]->GetCheckStatus())
				{
					m_pButton5[0]->SetCheckStatus(false);
					m_pButton5[1]->SetCheckStatus(false);
					SetButtonText(0);
				}
				else
				{
					m_pButton5[5]->SetCheckStatus(false);
					m_pButton5[2]->SetCheckStatus(false);
					m_pButton5[0]->SetCheckStatus(true);
					m_pButton5[1]->SetCheckStatus(true);
					SetButtonText(1);
				}
				return;
			case GUI_KEYBOARD_CONTROL_UMSCHALT_PRESSED:
				if (m_pButton5[2]->GetCheckStatus())
				{
					m_pButton5[2]->SetCheckStatus(false);
					SetButtonText(0);
				}
				else
				{
					m_pButton5[5]->SetCheckStatus(false);
					m_pButton5[2]->SetCheckStatus(true);
					m_pButton5[0]->SetCheckStatus(false);
					m_pButton5[1]->SetCheckStatus(false);
					SetButtonText(1);
				}
				return;
			case GUI_KEYBOARD_CONTROL_ALT:
				if (m_pButton5[5]->GetCheckStatus())
				{
					m_pButton5[5]->SetCheckStatus(false);
					SetButtonText(0);
				}
				else
				{
					m_pButton5[5]->SetCheckStatus(true);
					m_pButton5[2]->SetCheckStatus(false);
					m_pButton5[0]->SetCheckStatus(false);
					m_pButton5[1]->SetCheckStatus(false);
					SetButtonText(2);
				}
				return;
			case GUI_KEYBOARD_CONTROL_DELETE:
				RemoveCharacter();
				return;
			case GUI_THEME_SALLY_CLOSE:
				m_pParent->SendMessageToParent(this, 0, MS_SALLY_HIDE_KEYBOARD_CANCEL);
				return;
			}
		}
		ProcessSmartKeyboard();
		return;
	}
	SallyAPI::GUI::CPopUpWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

bool CKeyboard::CheckMaxLength()
{
	if ((((int) m_pEditInput->GetText().length()) >= m_iMaxCharacterInputLength) && (m_iMaxCharacterInputLength > -1))
	{
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::Config::CLanguageManager* languageManager = config->GetLanguageLocalization();

		std::string infoMessage = languageManager->GetString("This control allows only %s digits.", SallyAPI::String::StringHelper::ConvertToString(m_iMaxCharacterInputLength).c_str(), NULL);

		SallyAPI::GUI::SendMessage::CParameterInfoPopup sendMessageParameterInfoPopup(GUI_THEME_SALLY_ICON_MB_INFO, "Keyboard Info",
			infoMessage);
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_SHOW_INFO_POPUP, &sendMessageParameterInfoPopup);

		return false;
	}
	return true;
}

void CKeyboard::OnCommandChangeSign()
{
	std::string text = m_pEditInput->GetText();

	if (text.length() > 0)
	{
		if (text[0] == '-')
		{
			std::string temp = text.erase(0, 1);
			m_pEditInput->SetText(temp);
			return;
		}
	}
	std::string temp = "-";
	temp.append(text);

	m_pEditInput->SetText(temp);
}

void CKeyboard::RemoveCharacter()
{
	std::string text = m_pEditInput->GetText();

	if (text.length() < 1)
		return;

	std::string temp = text.erase(text.length() - 1, 1);
	m_pEditInput->SetText(temp);

	ProcessSmartKeyboard();
}

std::string CKeyboard::GetText()
{
	return m_pEditInput->GetText();
}

void CKeyboard::SetText(const std::string& text)
{
	m_pEditInput->SetText(text);
}

void CKeyboard::SetNumberOnly(bool numberOnly)
{
	m_bNumberOnly = numberOnly;
	if (m_bNumberOnly)
	{
		m_pKeyboardGroupBox->Visible(false);
		m_pNumberPadGroupBox->Visible(true);

		m_pEditInput->Move((WINDOW_WIDTH - 240) / 2 + 20, WINDOW_HEIGHT - 360 + 20 + 5);
		m_pEditInput->Resize(130, CONTROL_HEIGHT);
		m_pEditInput->SetAlign(DT_VCENTER | DT_RIGHT);
	}
	else
	{
		m_pKeyboardGroupBox->Visible(true);
		m_pNumberPadGroupBox->Visible(false);

		m_pEditInput->Move((WINDOW_WIDTH - 630) / 2 + 20, WINDOW_HEIGHT - 310 + 20 + 5);
		m_pEditInput->Resize(530, CONTROL_HEIGHT);
		m_pEditInput->SetAlign(DT_VCENTER | DT_LEFT);
	}
}

void CKeyboard::SetButtonText(int i)
{
	SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
	SallyAPI::Config::CKeyboardLayout* keyboardLayout = config->GetKeyboardLayout();

	std::vector<std::string> layout = keyboardLayout->GetKeyboardLayout();
	std::vector<std::string>::iterator iter = layout.begin();

	// Set the active text
	int q = i;
	if (q == 2)
		q++;

	for (int l = 0; l < 13; ++l)
		m_pButton1[l]->SetFourTextActive(q);
	for (int l = 0; l < 12; ++l)
		m_pButton2[l]->SetFourTextActive(q);
	for (int l = 0; l < 12; ++l)
		m_pButton3[l]->SetFourTextActive(q);
	for (int l = 0; l < 11; ++l)
		m_pButton4[l]->SetFourTextActive(q);

	// now set the text to the button
	for (int y = 0; y < 4; y++)
	{
		if (y == 2)
		{
			for (int l = 0; l < 13; ++l)
				m_pButton1[l]->SetFourTexts(y, " ");
			for (int l = 0; l < 12; ++l)
				m_pButton2[l]->SetFourTexts(y, " ");
			for (int l = 0; l < 12; ++l)
				m_pButton3[l]->SetFourTexts(y, " ");
			for (int l = 0; l < 11; ++l)
				m_pButton4[l]->SetFourTexts(y, " ");
			continue;
		}

		// fill the four lines
		std::string line = *iter;
		for (int l = 0; l < 13 && l < (int) line.length(); ++l)
		{
			std::string temp;
			temp.append(" ");
			temp[0] = line.at(l);
			m_pButton1[l]->SetFourTexts(y, temp);
		}
		++iter;

		line = *iter;
		for (int l = 0; l < 12 && l < (int) line.length(); ++l)
		{
			std::string temp;
			temp.append(" ");
			temp[0] = line.at(l);
			m_pButton2[l]->SetFourTexts(y, temp);
		}
		++iter;

		line = *iter;
		for (int l = 0; l < 12 && l < (int) line.length(); ++l)
		{
			std::string temp;
			temp.append(" ");
			temp[0] = line.at(l);
			m_pButton3[l]->SetFourTexts(y, temp);
		}
		++iter;

		line = *iter;
		for (int l = 0; l < 11 && l < (int) line.length(); ++l)
		{
			std::string temp;
			temp.append(" ");
			temp[0] = line.at(l);
			m_pButton4[l]->SetFourTexts(y, temp);
		}
		++iter;
	}


	// set now the text / with this value it will be worked
	iter = layout.begin();

	i *= 4;
	int k = 0;
	while (iter != layout.end() && k < i)
	{
		++iter;
		++k;
	}
	if (iter == layout.end())
		return;

	std::string line = *iter;
	for (int l = 0; l < 13 && l < (int) line.length(); ++l)
	{
		std::string temp;
		temp.append(" ");
		temp[0] = line.at(l);
		m_pButton1[l]->SetText(temp);
	}
	++iter;

	if (iter == layout.end())
		return;

	line = *iter;
	for (int l = 0; l < 12 && l < (int) line.length(); ++l)
	{
		std::string temp;
		temp.append(" ");
		temp[0] = line.at(l);
		m_pButton2[l]->SetText(temp);
	}
	++iter;

	line = *iter;
	for (int l = 0; l < 12 && l < (int) line.length(); ++l)
	{
		std::string temp;
		temp.append(" ");
		temp[0] = line.at(l);
		m_pButton3[l]->SetText(temp);
	}
	++iter;

	line = *iter;
	for (int l = 0; l < 11 && l < (int) line.length(); ++l)
	{
		std::string temp;
		temp.append(" ");
		temp[0] = line.at(l);
		m_pButton4[l]->SetText(temp);
	}
	++iter;
}

void CKeyboard::Visible(bool visible)
{
	SallyAPI::GUI::CControl::Visible(visible);
	
	SetButtonText(0);
	HideSmartKeyboard();
	m_pButton5[2]->SetCheckStatus(false);
}

void CKeyboard::OnCommandSmartKeyboard(SallyAPI::GUI::CGUIBaseObject* reporter)
{
	std::string labelText = reporter->GetText();
	if (labelText.length() > 0)
	{
		if (labelText.at(0) == '.')
			labelText = labelText.substr(1);
		if (labelText.at(0) == ' ')
			labelText = labelText.substr(1);
		if (labelText.at(0) == ',')
			labelText = labelText.substr(1);

		if (labelText.length() > 0)
		{
			std::string text = m_pEditInput->GetText();
			std::string lastWord = GetLastWord(text);

			std::string::size_type spacePos = text.find_last_of(lastWord);
			if(spacePos != std::string::npos)
			{
				text = text.substr(0, spacePos - lastWord.length() + 1);
			}
			text.append(labelText);

			m_pEditInput->SetText(text);

			HideSmartKeyboard();
		}
	}
}

std::string CKeyboard::FindLastTextBeforeSign(const std::string& input, const std::string& sign)
{
	std::string result = input;
	std::string::size_type spacePos = result.find_last_of(sign);
	if(spacePos != std::string::npos)
	{
		result = result.substr(spacePos + 1);
	}
	return result;
}

std::string CKeyboard::GetLastWord(const std::string& input)
{
	std::string result = input;
	result = FindLastTextBeforeSign(result, " ");
	result = FindLastTextBeforeSign(result, ".");
	result = FindLastTextBeforeSign(result, ",");

	return result;
}

void CKeyboard::HideSmartKeyboard()
{
	for (int i = 0; i < SMART_KEYBOARD_COMPLETE_COUNT; ++i)
	{
		m_pSmartKeyboardLabel[i]->SetText("");
		m_pSmartKeyboardLabel[i]->Visible(false);		
	}
	m_pSmartKeyboardGroupBox->Visible(false);

	m_iSmartKeyboardUsed = 0;
}

void CKeyboard::ProcessSmartKeyboard()
{
	if ((m_bPassword) || (m_bNumberOnly))
		return;

	std::string inputText = m_pEditInput->GetText();
	inputText = GetLastWord(inputText);
	inputText = SallyAPI::String::StringHelper::StringToLower(inputText);

	HideSmartKeyboard();

	if (inputText.length() == 0)
		return;

	std::vector<std::string> sentencelist = SearchSentenceInDatabase(inputText);
	std::vector<std::string> wordlist = SearchWordInDatabase(inputText);

	if (m_bEnableKeyboardBrainHistory)
	{
		// get sentences from database
		std::vector<std::string>::iterator sentencelistIterator = sentencelist.begin();

		while(sentencelistIterator != sentencelist.end())
		{
			std::string temp = *sentencelistIterator;

			m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->Visible(true);
			m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->SetText(temp);
			++m_iSmartKeyboardUsed;
			++sentencelistIterator;
		}

		// get words from database
		std::vector<std::string>::iterator wordlistIterator = wordlist.begin();

		while(wordlistIterator != wordlist.end())
		{
			std::string temp = *wordlistIterator;

			m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->Visible(true);
			m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->SetText(temp);
			++m_iSmartKeyboardUsed;
			++wordlistIterator;
		}
	}

	if (m_bEnableKeyboardBrainWordList)
	{
		// get from word list
		SallyAPI::Config::CConfig* config = SallyAPI::Config::CConfig::GetInstance();
		SallyAPI::Config::CSmartKeyboard* smartKeyboard = config->GetSmartKeyboard();

		std::vector<std::string>& smartlist = smartKeyboard->GetSmartKeyboard();
		std::vector<std::string>::iterator smartlistIter = smartlist.begin();

		while(smartlistIter != smartlist.end() && m_iSmartKeyboardUsed < SMART_KEYBOARD_COMPLETE_COUNT)
		{
			std::string temp = *smartlistIter;
			temp = SallyAPI::String::StringHelper::StringToLower(temp);

			if (temp.length() > inputText.length())
			{
				std::string::size_type pos = temp.find(inputText, 0);

				if((pos != std::string::npos) && (pos == 0))
				{
					std::vector<std::string>::iterator wordlistIterator = wordlist.begin();

					bool found = false;
					while ((wordlistIterator != wordlist.end()) && (!found))
					{
						std::string wordlistTemp = *wordlistIterator;

						if (wordlistTemp.compare(temp) == 0)
						{
							found = true;
						}
						++wordlistIterator;
					}

					if (!found)
					{
						m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->Visible(true);
						m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->SetText(temp);
						++m_iSmartKeyboardUsed;
					}
				} 
			}
			++smartlistIter;
		}
	}

	if (m_bEnableKeyboardWordRequest)
	{
		SallyAPI::GUI::SendMessage::CParameterString messageParamter(inputText);
		m_pParent->SendMessageToParent(this, 0, MS_SALLY_KEYBOARD_REQUEST_WORDS, &messageParamter);
	}

	if (m_iSmartKeyboardUsed > 0)
	{
		m_pSmartKeyboardGroupBox->Visible(true);
	}
}

void CKeyboard::CheckDatabaseExists()
{
	m_strMediaDirectory = SallyAPI::Core::CGame::GetMediaFolder();
	m_strMediaDirectory.append("keyboard.db");

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strMediaDirectory);

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strMediaDirectory);

	dbconn->LockDatabase();
	if (bFileExists == false)
	{
		// Create
		std::string queryCreateWords;
		queryCreateWords.append("CREATE TABLE words ( \
						   Word				TEXT UNIQUE, \
						   UsedCount		INTEGER \
						   );");

		try
		{
			SallyAPI::Database::CStatement* stmtCreate = dbconn->CreateStatement();
			stmtCreate->Execute(queryCreateWords.c_str());
		}
		catch (SallyAPI::Database::CSQLException* e)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error(e->GetMessage());
		}

		// Create
		std::string queryCreateSentence;
		queryCreateSentence.append("CREATE TABLE sentences ( \
						   Sentence			TEXT UNIQUE, \
						   UsedCount		INTEGER \
						   );");

		try
		{
			SallyAPI::Database::CStatement* queryCreate = dbconn->CreateStatement();
			queryCreate->Execute(queryCreateSentence.c_str());
		}
		catch (SallyAPI::Database::CSQLException* e)
		{
			SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
			logger->Error(e->GetMessage());
		}
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strMediaDirectory);
}

std::vector<std::string> CKeyboard::SearchWordInDatabase(const std::string& searchFor)
{
	std::vector<std::string> wordlist;

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strMediaDirectory);
	if (!bFileExists)
		return wordlist;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strMediaDirectory);

	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchFor, "'", "#");

	std::string queryWord;
	queryWord.append("SELECT Word FROM words WHERE Word LIKE '");
	queryWord.append(searchSearchFor);
	queryWord.append("%' ORDER BY UsedCount DESC LIMIT 5;");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(queryWord.c_str());

		while (rslt->Next())
		{
			std::string sDBWord = rslt->GetString(1);

			sDBWord = SallyAPI::String::StringHelper::ReplaceString(sDBWord, "#", "'");

			wordlist.push_back(sDBWord);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strMediaDirectory);

	return wordlist;
}


std::vector<std::string> CKeyboard::SearchSentenceInDatabase(const std::string& searchFor)
{
	std::vector<std::string> wordlist;

	bool bFileExists = SallyAPI::File::FileHelper::FileExistsAndNotEmpty(m_strMediaDirectory);
	if (!bFileExists)
		return wordlist;

	SallyAPI::Database::CDatabaseConnection* dbconn = SallyAPI::Database::CDatabaseConnection::Open(m_strMediaDirectory);

	std::string searchSearchFor = SallyAPI::String::StringHelper::ReplaceString(searchFor, "'", "#");

	std::string querySentence;
	querySentence.append("SELECT Sentence FROM sentences WHERE Sentence LIKE '");
	querySentence.append(searchSearchFor);
	querySentence.append("%' ORDER BY UsedCount DESC LIMIT 5;");

	dbconn->LockDatabase();
	SallyAPI::Database::CStatement* stmt = dbconn->CreateStatement();

	try
	{
		SallyAPI::Database::CResultSet* rslt = stmt->ExecuteQuery(querySentence.c_str());

		while (rslt->Next())
		{
			std::string sDBWord = rslt->GetString(1);

			sDBWord = SallyAPI::String::StringHelper::ReplaceString(sDBWord, "#", "'");

			wordlist.push_back(sDBWord);
		}
	}
	catch (SallyAPI::Database::CSQLException* e)
	{
		SallyAPI::System::CLogger* logger = SallyAPI::Core::CGame::GetLogger();
		logger->Error(e->GetMessage());
	}
	dbconn->ReleaseDatabase();

	SallyAPI::Database::CDatabaseConnection::Close(m_strMediaDirectory);

	return wordlist;
}

void CKeyboard::CharInputPressed(char c)
{
	switch (c)
	{
	case 13:
		SendMessageToParent(this, GUI_KEYBOARD_CONTROL_ENTER, GUI_BUTTON_CLICKED);
		break;
	case 8:
		RemoveCharacter();
		break;
	default:
		if ((c < 30) && (c >= 0))
			return;

		if (m_bNumberOnly)
		{
			if ((c == '-') || (c == '+'))
			{
				OnCommandChangeSign();
				return;
			}

			// we have a number only input field... is only a number pressed?
			if ((c < 48) || (c > 57))
				return;
		}

		if (CheckMaxLength())
		{
			std::string text = m_pEditInput->GetText();

			text.append(" ");
			text.at(text.length() - 1) = c;
			m_pEditInput->SetText(text);
		}
		break;
	}
	
	ProcessSmartKeyboard();
}

void CKeyboard::SetData(SallyAPI::GUI::CEdit* editControl)
{
	m_iMaxCharacterInputLength = editControl->GetMaxCharacterInputLength();

	if (editControl->GetNumberOnly())
	{
		SetNumberOnly(true);
		
		if (m_iMaxCharacterInputLength > 11)
			m_iMaxCharacterInputLength = 11;
	}
	else
	{
		SetNumberOnly(false);
	}

	m_bEnableKeyboardBrainHistory = editControl->GetEnableKeyboardBrainHistory();
	m_bEnableKeyboardBrainWordList = editControl->GetEnableKeyboardBrainWordList();
	m_bEnableKeyboardWordRequest = editControl->GetEnableKeyboardWordRequest();

	m_bPassword = editControl->GetPassword();
	if (m_bPassword)
	{
		// set text only if it is not a password field
		m_pEditInput->SetPassword(true);
		SetText("");
	}
	else
	{
		// set text only if it is not a password field
		m_pEditInput->SetPassword(false);
		SetText(editControl->GetText());
	}
}

void CKeyboard::SetRequestWordResult(SallyAPI::GUI::SendMessage::CParameterKeyboardRequestWords& messageParamter)
{
	std::vector<std::string> result = messageParamter.GetResult();
	std::vector<std::string>::iterator smartlistIter = result.begin();

	while(smartlistIter != result.end() && m_iSmartKeyboardUsed < SMART_KEYBOARD_COMPLETE_COUNT)
	{
		std::string temp = *smartlistIter;

		m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->Visible(true);
		m_pSmartKeyboardLabel[m_iSmartKeyboardUsed]->SetText(temp);
		++m_iSmartKeyboardUsed;

		++smartlistIter;
	}
}