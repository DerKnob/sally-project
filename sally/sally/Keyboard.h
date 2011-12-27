////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sally\Keyboard.h
///
/// \brief	Declares the keyboard class. 
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
#include "KeyboardBrain.h"

#define SMART_KEYBOARD_ROW_COUNT		6
#define SMART_KEYBOARD_COMPLETE_COUNT	(SMART_KEYBOARD_ROW_COUNT * 3)

class CKeyboard
	: public SallyAPI::GUI::CPopUpWindow
{
private:
	std::string				m_strMediaDirectory;
	CKeyboardBrain			m_tKeyboardBrain;
	int						m_iSmartKeyboardUsed;

	SallyAPI::GUI::CEdit*	m_pEditInput;
	SallyAPI::GUI::CButton*	m_pButton1[13];
	SallyAPI::GUI::CButton*	m_pButton2[12];
	SallyAPI::GUI::CButton*	m_pButton3[12];
	SallyAPI::GUI::CButton*	m_pButton4[11];
	SallyAPI::GUI::CButton*	m_pButton5[8];

	SallyAPI::GUI::CButton*	m_pButtonNumber[14];

	SallyAPI::GUI::CLabel*	m_pSmartKeyboardLabel[SMART_KEYBOARD_COMPLETE_COUNT];
	bool					m_bNumberOnly;
	bool					m_bPassword;
	bool					m_bEnableKeyboardBrainHistory;
	bool					m_bEnableKeyboardBrainWordList;
	bool					m_bEnableKeyboardWordRequest;
	int						m_iMaxCharacterInputLength;

	SallyAPI::GUI::CGroupBox*	m_pNumberPadGroupBox;
	SallyAPI::GUI::CGroupBox*	m_pKeyboardGroupBox;
	SallyAPI::GUI::CGroupBox*	m_pSmartKeyboardGroupBox;

	bool		CheckMaxLength();
	void		SetButtonText(int i);
	void		ProcessSmartKeyboard();
	void		RemoveCharacter();
	void		OnCommandChangeSign();
	void		HideSmartKeyboard();
	void		OnCommandSmartKeyboard(SallyAPI::GUI::CGUIBaseObject* reporter);
	std::string FindLastTextBeforeSign(const std::string& input, const std::string& sign);
	std::string GetLastWord(const std::string& input);
	void		SetNumberOnly(bool numberOnly);

	void						CheckDatabaseExists();
	std::vector<std::string>	SearchWordInDatabase(const std::string& searchFor);
	std::vector<std::string>	SearchSentenceInDatabase(const std::string& searchFor);
public:
	CKeyboard(SallyAPI::GUI::CGUIBaseObject* parent);
	virtual ~CKeyboard();

	virtual void	SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
	virtual void	Visible(bool visible);

	std::string		GetText();
	void			SetText(const std::string& text);

	void			CharInputPressed(char c);
	void			SetData(SallyAPI::GUI::CEdit* editControl);

	void			SetRequestWordResult(SallyAPI::GUI::SendMessage::CParameterKeyboardRequestWords& messageParamter);
};
