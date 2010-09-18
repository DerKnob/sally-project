////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\NumberSelector.cpp
///
/// \brief	Implements the number selector class. 
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

#include "NumberSelector.h"

using namespace SallyAPI::GUI;

#define GUI_EDIT_CONTROL	50000
#define GUI_BUTTON_UP		50001
#define GUI_BUTTON_DOWN		50002

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CNumberSelector::CNumberSelector(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y,
/// int width, int controlId) :SallyAPI::GUI::CForm(parent, x, y, width, CONTROL_HEIGHT,
/// controlId), m_iMaxValue(10), m_iMinValue(1), m_iSteps(1), m_iStepProcessed(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	x				The x coordinate. 
/// \param	y				The y coordinate. 
/// \param	width			The width. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CNumberSelector::CNumberSelector(SallyAPI::GUI::CGUIBaseObject* parent, int x, int y, int width, int controlId)
	:SallyAPI::GUI::CForm(parent, x, y, width, CONTROL_HEIGHT, controlId),
	m_iMaxValue(10), m_iMinValue(1), m_iSteps(1), m_iStepProcessed(false)
{
	m_pButtonUp = new SallyAPI::GUI::CButton(this, 0, 0,
		CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_BUTTON_UP);
	m_pButtonUp->SetImageId(GUI_THEME_SALLY_ICON_UP);
	this->AddChild(m_pButtonUp);

	m_pEdit = new SallyAPI::GUI::CEdit(this, CONTROL_HEIGHT, 0, m_iWidth - CONTROL_HEIGHT * 2, GUI_EDIT_CONTROL);
	m_pEdit->SetAlign(DT_CENTER | DT_VCENTER);
	m_pEdit->SetNumberOnly(true);
	this->AddChild(m_pEdit);

	m_pButtonDown = new SallyAPI::GUI::CButton(this, m_iWidth - CONTROL_HEIGHT, 0,
		CONTROL_HEIGHT, CONTROL_HEIGHT, GUI_BUTTON_DOWN);
	m_pButtonDown->SetImageId(GUI_THEME_SALLY_ICON_DOWN);
	this->AddChild(m_pButtonDown);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CNumberSelector::~CNumberSelector()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CNumberSelector::~CNumberSelector()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CNumberSelector::GetMinValue()
///
/// \brief	Gets the minimum value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The minimum value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CNumberSelector::GetMinValue()
{
	return m_iMinValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::SetMinValue(int value)
///
/// \brief	Sets a minimum value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::SetMinValue(int value)
{
	m_iMinValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CNumberSelector::GetMaxValue()
///
/// \brief	Gets the maximum value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The maximum value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CNumberSelector::GetMaxValue()
{
	return m_iMaxValue;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::SetMaxValue(int value)
///
/// \brief	Sets a maximum value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::SetMaxValue(int value)
{
	m_iMaxValue = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CNumberSelector::GetSteps()
///
/// \brief	Gets the steps. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The steps. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CNumberSelector::GetSteps()
{
	return m_iSteps;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::SetSteps(int value)
///
/// \brief	Sets the steps. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::SetSteps(int value)
{
	m_iSteps = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::Timer(float timeDelta)
///
/// \brief	Timers. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	timeDelta	The time delta. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::Timer(float timeDelta)
{
	SallyAPI::GUI::CForm::Timer(timeDelta);

	if (!m_bMouseDown)
	{
		m_fStartTime = 0;
		return;
	}

	if (m_fStartTime == 0)
		m_fStartTime = m_fTimeDelta;

	if (m_fStartTime < m_fTimeDelta - 0.3)
	{
		if (m_pButtonUp->IsMouseDown())
			OnCommandButtonUp(m_iSteps);
		else if (m_pButtonDown->IsMouseDown())
			OnCommandButtonDown(m_iSteps);

		m_fStartTime = 0;
		m_iStepProcessed = true;
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_EDIT_CHANGED:
		switch (reporterId)
		{
		case GUI_EDIT_CONTROL:
			OnCommandEditChanged();
			return;
		}
		return;
	case GUI_BUTTON_CLICKED:
		if (m_iStepProcessed == true)
		{
			m_iStepProcessed = false;
			return;
		}
		switch (reporterId)
		{
		case GUI_BUTTON_UP:
			OnCommandButtonUp(1);
			return;
		case GUI_BUTTON_DOWN:
			OnCommandButtonDown(1);
			return;
		}
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::OnCommandEditChanged()
///
/// \brief	Executes the command edit changed action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::OnCommandEditChanged()
{
	int value = SallyAPI::String::StringHelper::ConvertToInt(m_pEdit->GetText());

	if (value > m_iMaxValue)
	{
		m_pEdit->SetText(SallyAPI::String::StringHelper::ConvertToString(m_iMaxValue));
	}
	else if (value < m_iMinValue)
	{
		m_pEdit->SetText(SallyAPI::String::StringHelper::ConvertToString(m_iMinValue));
	}
	else if ((value != 0) && (m_pEdit->GetText().find_first_of('0') == 0))
	{
		m_pEdit->SetText(SallyAPI::String::StringHelper::ConvertToString(value));
	}
	else if ((value == 0) && (m_pEdit->GetText().length() > 1))
	{
		m_pEdit->SetText("0");
	}
	else if ((value == 0) && (m_pEdit->GetText().length() == 0))
	{
		m_pEdit->SetText("0");
	}
	SendMessageToParent(this, m_iControlId, GUI_NUMBER_SELECTOR_CHANGED);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::OnCommandButtonUp(int step)
///
/// \brief	Executes the command button up action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	step	Amount to increment by. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::OnCommandButtonUp(int step)
{
	int value = SallyAPI::String::StringHelper::ConvertToInt(m_pEdit->GetText());

	value += step;

	if (value > m_iMaxValue)
		value = m_iMaxValue;

	m_pEdit->SetText(SallyAPI::String::StringHelper::ConvertToString(value));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::OnCommandButtonDown(int step)
///
/// \brief	Executes the command button down action. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	step	Amount to increment by. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::OnCommandButtonDown(int step)
{
	int value = SallyAPI::String::StringHelper::ConvertToInt(m_pEdit->GetText());

	value -= step;

	if (value <= m_iMinValue)
		value = m_iMinValue;
	m_pEdit->SetText(SallyAPI::String::StringHelper::ConvertToString(value));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::SetValue(int value)
///
/// \brief	Sets a value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	value	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::SetValue(int value)
{
	m_pEdit->SetText(SallyAPI::String::StringHelper::ConvertToString(value));
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	int CNumberSelector::GetValue()
///
/// \brief	Gets the value. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \return	The value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int CNumberSelector::GetValue()
{
	return SallyAPI::String::StringHelper::ConvertToInt(m_pEdit->GetText());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CNumberSelector::Enable(bool enable)
///
/// \brief	Enables. 
///
/// \author	Christian Knobloch
/// \date	19.04.2010
///
/// \param	enable	true to enable, false to disable. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CNumberSelector::Enable(bool enable)
{
	SallyAPI::GUI::CForm::Enable(enable);

	// Send Timer Event to all Childs
	std::list<CControl*>::iterator iter = m_GUIControlList.begin();
	while (iter !=  m_GUIControlList.end())
	{
		CControl* control = *iter;
		control->Enable(enable);
		++iter;
	}
}