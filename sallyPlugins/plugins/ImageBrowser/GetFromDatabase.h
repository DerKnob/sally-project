////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\GetFromDatabase.h
///
/// \brief	Declares the get from database class. 
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

#pragma once
#include "Define.h"
#include <sallyAPI\sallyAPI.h>
#include "ImageDatabase.h"

class CGetFromDatabase
		: public SallyAPI::System::CThread
{
private:
	std::vector<std::string>*			m_vpFolderPictureList;
	SallyAPI::GUI::CForm*				m_pParentControl;
	SallyAPI::GUI::CApplicationWindow*	m_pApplicationWindow;
	std::string							m_sYear;
	std::string							m_sMonth;

	virtual void RunEx();
public:
	CGetFromDatabase();
	virtual ~CGetFromDatabase();

	void SetValues(std::vector<std::string>* folderPictureList, SallyAPI::GUI::CApplicationWindow* applicationWindow,
		SallyAPI::GUI::CForm* control, const std::string& year, const std::string& month);
};
