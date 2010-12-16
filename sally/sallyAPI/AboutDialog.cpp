////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	sallyAPI\AboutDialog.cpp
///
/// \brief	Implements the about Dialog. 
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

#include "AboutDialog.h"

using namespace SallyAPI::GUI;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CAboutDialog::CAboutDialog(SallyAPI::GUI::CApplicationWindow* parent, int graphicId,
/// const std::string& explicidAppName,
/// const std::string& aboutText) :SallyAPI::GUI::CPopUpWindow(parent, graphicId,
/// explicidAppName), m_pApplication(parent)
///
/// \brief	Constructor. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
///
/// \param [in,out]	parent	If non-null, the parent. 
/// \param	graphicId		Identifier for the graphic. 
/// \param	explicidAppName	Name of the explicid application. 
/// \param	aboutText		The about text. 
////////////////////////////////////////////////////////////////////////////////////////////////////

CAboutDialog::CAboutDialog(SallyAPI::GUI::CApplicationWindow* parent, int graphicId, const std::string& explicidAppName, const std::string& aboutText)
	:SallyAPI::GUI::CPopUpWindow(parent, graphicId, explicidAppName), m_pApplication(parent)
{
	m_pBackgroundGroup = new SallyAPI::GUI::CGroupBox(this, (WINDOW_WIDTH - 500) / 2,
		(WINDOW_HEIGHT - 350) / 2, 500, 350);
	this->AddChild(m_pBackgroundGroup);

	m_pImage = new SallyAPI::GUI::CImageBox(m_pBackgroundGroup, -20, -20, 64, 64);
	m_pImage->SetImageId(GetGraphicId());
	m_pBackgroundGroup->AddChild(m_pImage);

	m_pTitle = new SallyAPI::GUI::CLabel(m_pBackgroundGroup, 55, 20, 320);
	m_pTitle->SetLocalised(false);
	m_pTitle->SetText("About " + m_pApplication->GetAppName());
	m_pTitle->SetFont("big2.font");
	m_pBackgroundGroup->AddChild(m_pTitle);

	m_pVersion = new SallyAPI::GUI::CLabel(m_pBackgroundGroup, 55 + 320, 20, 100);
	m_pVersion->SetLocalised(false);
	m_pVersion->SetText("v. " + m_pApplication->GetVersion());
	m_pVersion->SetAlign(DT_RIGHT | DT_VCENTER);
	m_pBackgroundGroup->AddChild(m_pVersion);

	m_pAuthor = new SallyAPI::GUI::CLabel(m_pBackgroundGroup, 55, 20 + CONTROL_HEIGHT, 300);
	m_pAuthor->SetLocalised(false);
	m_pAuthor->SetText("by " + m_pApplication->GetAuthor());
	m_pBackgroundGroup->AddChild(m_pAuthor);

	m_pAbout = new SallyAPI::GUI::CLabelBox(m_pBackgroundGroup, 20, 100, 460, 350 - 90 - 20 - 10);
	m_pAbout->SetText(aboutText);
	m_pBackgroundGroup->AddChild(m_pAbout);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \fn	CAboutDialog::~CAboutDialog()
///
/// \brief	Destructor. 
///
/// \author	Christian Knobloch
/// \date	08.09.2010
////////////////////////////////////////////////////////////////////////////////////////////////////

CAboutDialog::~CAboutDialog()
{
}
