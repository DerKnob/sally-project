////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\GetFromDatabase.cpp
///
/// \brief	Implements the get from database class. 
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

#include "GetFromDatabase.h"

CGetFromDatabase::CGetFromDatabase()
{
}

CGetFromDatabase::~CGetFromDatabase()
{
}

void CGetFromDatabase::SetValues(std::vector<std::string>* folderPictureList, SallyAPI::GUI::CApplicationWindow* applicationWindow,
								SallyAPI::GUI::CForm* control, const std::string& year, const std::string& month)
{
	m_vpFolderPictureList = folderPictureList;
	m_pParentControl = control;
	m_sYear = year;
	m_sMonth = month;
	m_pApplicationWindow = applicationWindow;
}

void CGetFromDatabase::RunEx()
{
	CImageDatabase::GetImagesFromDatabase(m_pApplicationWindow, m_vpFolderPictureList, m_sYear, m_sMonth);

	m_pParentControl->SendMessageToParent(m_pParentControl, 0, GUI_APP_DATABASE_LOAD_DONE);
}