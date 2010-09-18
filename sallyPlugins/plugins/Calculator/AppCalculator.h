////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	Calculator\AppCalculator.h
///
/// \brief	Declares the application calculator class. 
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

#pragma once
#include <sallyAPI\sallyAPI.h>
#include "Define.h"

class CAppCalculator :
	public SallyAPI::GUI::CApplicationWindow
{
private:
	SallyAPI::GUI::CForm*	m_pCalculatorForm;
	SallyAPI::GUI::CButton*	m_pButtonNumbers[12];
	SallyAPI::GUI::CButton*	m_pButtonDelete;
	SallyAPI::GUI::CButton*	m_pButtonEquals;
	SallyAPI::GUI::CButton*	m_pButtonZero;
	SallyAPI::GUI::CButton*	m_pButtonDot;
	SallyAPI::GUI::CButton*	m_pButtonNegate;
	SallyAPI::GUI::CButton*	m_pButtonSubtraction;
	SallyAPI::GUI::CButton*	m_pButtonMultiply;
	SallyAPI::GUI::CButton*	m_pButtonDivide;
	SallyAPI::GUI::CButton*	m_pButtonRemove;
	SallyAPI::GUI::CButton*	m_pButtonAddition;
	SallyAPI::GUI::CEdit*	m_pEditOutput;

	double					m_dCurrentValue;
	int						m_iActiveOperation;

public:
	CAppCalculator(SallyAPI::GUI::CGUIBaseObject* parent, int graphicId, const std::string& pluginPath);
	virtual ~CAppCalculator();

	virtual void SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter = NULL);
};
