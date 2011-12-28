////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScreensaverControls.cpp
///
/// \brief	Implements the screensaver controls class. 
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

#include "ScreensaverControls.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScreensaverControls::CScreensaverControls(SallyAPI::GUI::CGUIBaseObject* parent,
/// SallyAPI::GUI::CApplicationWindow* app) :SallyAPI::GUI::CForm(parent, 0, 0, 80, 80),
/// m_pApp(app), m_bShowAlways(false)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param [in,out]	app		If non-null, the application. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScreensaverControls::CScreensaverControls(SallyAPI::GUI::CGUIBaseObject* parent, SallyAPI::GUI::CApplicationWindow* app)
	:SallyAPI::GUI::CForm(parent, 0, 0, 80, 80), m_pApp(app), m_bShowAlways(false)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScreensaverControls::~CScreensaverControls()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

CScreensaverControls::~CScreensaverControls()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControl::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	27.12.2011
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControls::RenderControl()
{
	DrawImage(GUI_THEME_SCREENSAVER_CONTROL_APP, 0, 0, 80, 80);
	DrawImage(GUI_THEME_SCREENSAVER_CONTROL_ACTION, 80, 0, m_iWidth - 80, 80);

	DrawImage(m_pApp->GetGraphicId(), 8, 8, 64, 64);

	SallyAPI::GUI::CForm::RenderControl();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControls::AddChild(SallyAPI::GUI::CControl* control)
///
/// \brief	Adds a child. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \param [in,out]	control	If non-null, the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControls::AddChild(SallyAPI::GUI::CControl* control)
{
	m_vControl.push_back(control);

	// move new control
	control->Move(m_vControl.size() * 80, 0);

	// resize it self
	Resize(m_iWidth + control->GetWidth(), 80);

	SallyAPI::GUI::CForm::AddChild(control);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControls::ShowAlways(bool value)
///
/// \brief	Shows the always. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \param	value	true to value. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControls::ShowAlways(bool value)
{
	m_bShowAlways = value;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	bool CScreensaverControls::IsShowAlways()
///
/// \brief	Query if this object is show always. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \return	true if show always, false if not. 
////////////////////////////////////////////////////////////////////////////////////////////////////

bool CScreensaverControls::IsShowAlways()
{
	return m_bShowAlways;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	SallyAPI::GUI::CApplicationWindow* CScreensaverControls::GetApplicationWindow()
///
/// \brief	Gets the application window. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \return	null if it fails, else the application window. 
////////////////////////////////////////////////////////////////////////////////////////////////////

SallyAPI::GUI::CApplicationWindow* CScreensaverControls::GetApplicationWindow()
{
	return m_pApp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControls::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter,
/// int reporterId, int messageId, SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
///
/// \brief	Send message to parent. 
///
/// \author	Christian Knobloch
/// \date	28.12.2011
///
/// \param [in,out]	reporter			If non-null, the reporter. 
/// \param	reporterId					Identifier for the reporter. 
/// \param	messageId					Identifier for the message. 
/// \param [in,out]	messageParameter	If non-null, the message parameter. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControls::SendMessageToParent(SallyAPI::GUI::CGUIBaseObject* reporter, int reporterId, int messageId, 
											  SallyAPI::GUI::SendMessage::CParameterBase* messageParameter)
{
	switch (messageId)
	{
	case GUI_CONTROL_BLENDED:
		if ((reporter == this) && (GetAlphaBlending() == 0))
		{
			Visible(false);
			Enable(false);
		}
		return;
	}
	SallyAPI::GUI::CForm::SendMessageToParent(reporter, reporterId, messageId, messageParameter);
}