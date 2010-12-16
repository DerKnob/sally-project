////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\LoadImage.h
///
/// \brief	Declares the load image class. 
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
#include <sallyAPI\sallyAPI.h>

class CLoadImage
		: public SallyAPI::System::CThread
{
private:
	std::vector<std::string>*							m_vpFolderPictureList;
	std::map<std::string, SallyAPI::GUI::CPicture*>*	m_pmPictureVector;
	SallyAPI::GUI::CForm*								m_pParentControl;
	int*												m_piStartView;
	int													m_iCols;
	int													m_iRows;

	void LoadImage(const std::string& filename);
	void Load();
	void UnLoad();

	virtual void RunEx();
public:
	CLoadImage();
	virtual ~CLoadImage();

	void SetValues(std::vector<std::string>* folderPictureList,
		std::map<std::string, SallyAPI::GUI::CPicture*>* pictureVector, SallyAPI::GUI::CForm* control,
		int* startView, int cols, int rows);
};
