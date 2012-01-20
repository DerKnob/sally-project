////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\ScreensaverControlLabel.cpp
///
/// \brief	Implements the screensaver control label class. 
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

#include "ScreensaverControlLabel.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScreensaverControlLabel::CScreensaverControlLabel(SallyAPI::GUI::CGUIBaseObject* parent,
/// int controlId) :SallyAPI::GUI::CForm(parent, 0, 0, 80, 80, controlId)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	20.01.2012
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	controlId		Identifier for the control. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CScreensaverControlLabel::CScreensaverControlLabel(SallyAPI::GUI::CGUIBaseObject* parent, int controlId)
	:SallyAPI::GUI::CForm(parent, 0, 0, 160, 80, controlId)
{
	m_pLabelBox = new SallyAPI::GUI::CLabelBox(this, 4, 4, 160 - 4, 80 - 4);
	m_pLabelBox->SetFont("screensaver.font");
	m_pLabelBox->SetAlign(DT_CENTER | DT_VCENTER | DT_WORDBREAK);
	this->AddChild(m_pLabelBox);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CScreensaverControlLabel::~CScreensaverControlLabel()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	20.01.2012
////////////////////////////////////////////////////////////////////////////////////////////////////

CScreensaverControlLabel::~CScreensaverControlLabel()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControlLabel::SetText(const std::string& text)
///
/// \brief	Sets a text. 
///
/// \author	Christian Knobloch
/// \date	20.01.2012
///
/// \param	text	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControlLabel::SetText(const std::string& text)
{
	m_pLabelBox->SetText(text);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	std::string CScreensaverControlLabel::GetText()
///
/// \brief	Gets the text. 
///
/// \author	Christian Knobloch
/// \date	20.01.2012
///
/// \return	The text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

std::string CScreensaverControlLabel::GetText()
{
	return m_pLabelBox->GetText();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	void CScreensaverControlLabel::RenderControl()
///
/// \brief	Renders a control. 
///
/// \author	Christian Knobloch
/// \date	20.01.2012
////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreensaverControlLabel::RenderControl()
{
	DrawImage(GUI_THEME_SCREENSAVER_CONTROL_ACTION, 0, 0, 80, 80);
	DrawImage(GUI_THEME_SCREENSAVER_CONTROL_ACTION, 80, 0, 80, 80);

	SallyAPI::GUI::CForm::RenderControl();
}