////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	Calculator\AppCalculator.cpp
///
/// \brief	Implements the application calculator class. 
///
/// \author	Dominik Haselmeier
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

#include "AppCalculator.h"

char tempV[] = "321654987";

CAppCalculator::CAppCalculator(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath)
	:SallyAPI::GUI::CApplicationWindow(parent, graphicId, pluginPath)
{
	m_dCurrentValue = 0;
	m_iActiveOperation = 0;

	int l = 8;
	std::string temp;
	temp.append(" ");

	m_pCalculatorForm = new SallyAPI::GUI::CForm(this, (WINDOW_WIDTH - 190) / 2, (WINDOW_HEIGHT - 280) / 2, 190, 290);
	this->AddChild(m_pCalculatorForm);

	m_pEditOutput = new SallyAPI::GUI::CEdit(m_pCalculatorForm, 0, 5, 140,
		GUI_APP_CALCULATOR_ZERO);
	m_pEditOutput->SetLocalised(false);
	m_pEditOutput->SetText("0");
	m_pEditOutput->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pEditOutput->SetKeyboardInputEnabled(false);
	m_pCalculatorForm->AddChild(m_pEditOutput);

	m_pButtonRemove = new SallyAPI::GUI::CButton(m_pCalculatorForm, 150, 0, 40, 40,
		GUI_APP_CALCULATOR_REMOVE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonRemove->SetLocalised(false);
	m_pButtonRemove->SetImageId(GUI_THEME_SALLY_KEYBOARD_BACKSPACE);
	m_pCalculatorForm->AddChild(m_pButtonRemove);

	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			m_pButtonNumbers[l] = new SallyAPI::GUI::CButton(m_pCalculatorForm, (k * 50), 50 + (i * 50), 40, 40,
				GUI_APP_CALCULATOR + l + 1, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
			m_pButtonNumbers[l]->SetLocalised(false);
			temp[0] = tempV[l];
			m_pButtonNumbers[l]->SetText(temp);
			m_pButtonNumbers[l]->SetAlign(DT_CENTER | DT_VCENTER );
			m_pCalculatorForm->AddChild(m_pButtonNumbers[l]);
			l--;
		}
	}
	m_pButtonZero = new SallyAPI::GUI::CButton(m_pCalculatorForm, 0, 200, 40, 40,
		GUI_APP_CALCULATOR_ZERO, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonZero->SetLocalised(false);
	m_pButtonZero->SetText("0");
	m_pButtonZero->SetAlign(DT_CENTER | DT_VCENTER);
	m_pCalculatorForm->AddChild(m_pButtonZero);

	m_pButtonDot = new SallyAPI::GUI::CButton(m_pCalculatorForm, (1 * 50), 200, 40, 40,
		GUI_APP_CALCULATOR_DOT, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonDot->SetLocalised(false);
	m_pButtonDot->SetText(".");
	m_pButtonDot->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonDot);

	m_pButtonNegate = new SallyAPI::GUI::CButton(m_pCalculatorForm, (2 * 50), 200, 40, 40,
		GUI_APP_CALCULATOR_NEGATE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonNegate->SetLocalised(false);
	m_pButtonNegate->SetText("+/-");
	m_pButtonNegate->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonNegate);

	m_pButtonDelete = new SallyAPI::GUI::CButton(m_pCalculatorForm, 0, 250, 40, 40,
		GUI_APP_CALCULATOR_DELETE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonDelete->SetLocalised(false);
	m_pButtonDelete->SetImageId(GUI_THEME_SALLY_KEYBOARD_CLEAR);
	m_pButtonDelete->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonDelete);

	m_pButtonEquals = new SallyAPI::GUI::CButton(m_pCalculatorForm, 50, 250, 140, 40,
		GUI_APP_CALCULATOR_EQUALS, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonEquals->SetLocalised(false);
	m_pButtonEquals->SetText("=");
	m_pButtonEquals->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonEquals);


	// Commands
	m_pButtonAddition = new SallyAPI::GUI::CButton(m_pCalculatorForm, (3 * 50), 50, 40, 40,
		GUI_APP_CALCULATOR_ADDITION, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonAddition->SetLocalised(false);
	m_pButtonAddition->SetText("+");
	m_pButtonAddition->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonAddition); 

	m_pButtonSubtraction = new SallyAPI::GUI::CButton(m_pCalculatorForm, (3 * 50), 100, 40, 40,
		GUI_APP_CALCULATOR_SUBTRACT, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonSubtraction->SetLocalised(false);
	m_pButtonSubtraction->SetText("-");
	m_pButtonSubtraction->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonSubtraction);

	m_pButtonMultiply = new SallyAPI::GUI::CButton(m_pCalculatorForm, (3 * 50), 150, 40, 40,
		GUI_APP_CALCULATOR_MULTIPLY, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonMultiply->SetLocalised(false);
	m_pButtonMultiply->SetText("*");
	m_pButtonMultiply->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonMultiply);

	m_pButtonDivide = new SallyAPI::GUI::CButton(m_pCalculatorForm, (3 * 50), 200, 40, 40,
		GUI_APP_CALCULATOR_DIVIDE, SallyAPI::GUI::BUTTON_TYPE_NORMAL);
	m_pButtonDivide->SetLocalised(false);
	m_pButtonDivide->SetText("/");
	m_pButtonDivide->SetAlign(DT_CENTER | DT_VCENTER );
	m_pCalculatorForm->AddChild(m_pButtonDivide);
}


CAppCalculator::~CAppCalculator()
{
}


void CAppCalculator::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{

	switch (messageId)
	{
	case GUI_BUTTON_CLICKED:

		if (m_pEditOutput->GetText().length() == 0)
		{
			m_pEditOutput->SetText("0");
		}
		std::string number;
		double value = 0;

		switch (reporterId)
		{
		case GUI_APP_CALCULATOR_ADDITION:
			value = SallyAPI::String::StringHelper::ConvertToDouble(m_pEditOutput->GetText());
			m_dCurrentValue = value;
			number = "";
			m_pEditOutput->SetText(number);
			m_iActiveOperation = reporterId;
			return;
		case GUI_APP_CALCULATOR_MULTIPLY:
			value = SallyAPI::String::StringHelper::ConvertToDouble(m_pEditOutput->GetText());
			m_dCurrentValue = value;
			number = "";
			m_pEditOutput->SetText(number);
			m_iActiveOperation = reporterId;
			return;
		case GUI_APP_CALCULATOR_DIVIDE:
			value = SallyAPI::String::StringHelper::ConvertToDouble(m_pEditOutput->GetText());
			m_dCurrentValue = value;
			number = "";
			m_pEditOutput->SetText(number);
			m_iActiveOperation = reporterId;
			return;
		case GUI_APP_CALCULATOR_SUBTRACT:
			value = SallyAPI::String::StringHelper::ConvertToDouble(m_pEditOutput->GetText());
			m_dCurrentValue = value;
			number = "";
			m_pEditOutput->SetText(number);
			m_iActiveOperation = reporterId;
			return;
		case GUI_APP_CALCULATOR_EQUALS:
			value = SallyAPI::String::StringHelper::ConvertToDouble(m_pEditOutput->GetText());
			switch(m_iActiveOperation)
			{
				case GUI_APP_CALCULATOR_ADDITION: m_dCurrentValue+=value; break;
				case GUI_APP_CALCULATOR_MULTIPLY: m_dCurrentValue*=value; break;
				case GUI_APP_CALCULATOR_DIVIDE: m_dCurrentValue/=value; break;
				case GUI_APP_CALCULATOR_SUBTRACT: m_dCurrentValue-=value; break;
			}
			number = SallyAPI::String::StringHelper::ConvertDoubleToString(m_dCurrentValue);
			m_pEditOutput->SetText(number);
			m_dCurrentValue = 0;
			return;
		case GUI_APP_CALCULATOR_DELETE:
			m_dCurrentValue = 0;
			number = "";
			m_pEditOutput->SetText(number);
			return;
		case GUI_APP_CALCULATOR_ZERO:
			number = m_pEditOutput->GetText();

			number.append(" ");
			number[number.length() - 1] = '0';
			if(number[0] == '0' && number.length() > 1 && number[1] != '.'){
				number = "0";
				m_pEditOutput->SetText(number);
			} else {
				m_pEditOutput->SetText(number);
			}
			return;
		case GUI_APP_CALCULATOR_NEGATE:
			value = SallyAPI::String::StringHelper::ConvertToDouble(m_pEditOutput->GetText());
			m_dCurrentValue = value * -1;
			number = SallyAPI::String::StringHelper::ConvertDoubleToString(m_dCurrentValue);
			m_pEditOutput->SetText(number);
			return;
		case GUI_APP_CALCULATOR_REMOVE:
			number = m_pEditOutput->GetText();
			if(number.length() > 1){
			number = number.substr(0,number.length() - 1); 
			} else {
			number = "0"; 
			}
			value = SallyAPI::String::StringHelper::ConvertToDouble(number);
			m_dCurrentValue = value;
			m_pEditOutput->SetText(number);
			return;
		case GUI_APP_CALCULATOR_DOT:
			number = m_pEditOutput->GetText();
			number = number.append("."); 
			m_pEditOutput->SetText(number);
			return;
		}

		if (reporterId >= GUI_APP_CALCULATOR + 1 && reporterId <= GUI_APP_CALCULATOR + 12)
		{
			number = m_pEditOutput->GetText();
			number.append(" ");
			if(number.length() > 0 && number[0] == '0'){
				if(number.length() > 1 && number[1] == '.'){
				} else {
				number = " ";
				}
			}
			number[number.length() - 1] = tempV[reporterId - (GUI_APP_CALCULATOR + 1)];
			m_pEditOutput->SetText(number);
			return;
		}
		return;
	}

	SallyAPI::GUI::CApplicationWindow::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}