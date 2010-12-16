////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	ImageBrowser\LoadImage.cpp
///
/// \brief	Implements the load image class. 
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

#include "LoadImage.h"

#define MAX_LEFT_THRESHOLD	8
#define MAX_LEFT			1
#define MAX_RIGHT			8

#define DISPLAYCOUNT	(m_iCols * m_iRows)

CLoadImage::CLoadImage()
{
}

CLoadImage::~CLoadImage()
{
}

void CLoadImage::SetValues(std::vector<std::string>* folderPictureList,
						   std::map<std::string, SallyAPI::GUI::CPicture*>* pictureVector,
						   SallyAPI::GUI::CForm* control,
						   int* startView, int cols, int rows)
{
	m_vpFolderPictureList = folderPictureList;
	m_pmPictureVector = pictureVector;
	m_pParentControl = control;
	m_piStartView = startView;
	m_iCols = cols;
	m_iRows = rows;
}

void CLoadImage::RunEx()
{
	while (m_bPleaseStop == false)
	{
		Load();
		UnLoad();

		Sleep(100);
	}
}

void CLoadImage::LoadImage(const std::string& filename)
{
	if ((filename[filename.length() - 1] == '\\') || (filename.compare("..") == 0))
		return;

	SallyAPI::GUI::CPicture* newPic = new SallyAPI::GUI::CPicture();
	bool result = newPic->LoadTextureOverGDI(filename, PICTURE_WIDTH_MAX_SMALL, PICTURE_HEIGHT_MAX_SMALL);

	if (!result)
	{
		SafeDelete(newPic);
		return;
	}

	(*m_pmPictureVector)[filename] = newPic;

	SallyAPI::GUI::SendMessage::CParameterString message(filename);
	m_pParentControl->SendMessageToParent(0, 0, GUI_APP_FOLDER_PIC_LOADED, &message);
}

void CLoadImage::Load()
{
	int startViewTemp = *m_piStartView;

	int startItem = (startViewTemp - MAX_LEFT) * m_iRows;
	int endItem = ((startViewTemp - MAX_LEFT) * m_iRows) + DISPLAYCOUNT + (MAX_RIGHT * m_iRows);
	
	// now correct the startItem
	if (startItem < 0)
		startItem = 0;

	for (int i = startItem; (i <= endItem) && (!m_bPleaseStop) && (startViewTemp == *m_piStartView) && (i < (int) m_vpFolderPictureList->size()); ++i)
	{

		std::string filename = m_vpFolderPictureList->at(i);
		if ((*m_pmPictureVector)[filename] == NULL)
		{
			LoadImage(filename);
		}
	}
}

void CLoadImage::UnLoad()
{
	int startViewTemp = *m_piStartView;

	int startItem = (startViewTemp - MAX_LEFT_THRESHOLD) * m_iRows;
	int endItem = ((startViewTemp - MAX_LEFT) * m_iRows) + DISPLAYCOUNT + (MAX_RIGHT * m_iRows);

	// now correct the startItem
	if (startItem < 0)
		startItem = 0;

	// delete before
	for (int i = 0; (i < startItem) && (i < (int) m_vpFolderPictureList->size()); ++i)
	{
		m_pParentControl->SendMessageToParent(0, 0, GUI_APP_RENDER_LOCK);

		std::string filename = m_vpFolderPictureList->at(i);
		SallyAPI::GUI::CPicture* picture = (*m_pmPictureVector)[filename];

		if (picture != NULL)
		{
			(*m_pmPictureVector).erase(filename);
			SafeDelete(picture);
		}

		m_pParentControl->SendMessageToParent(0, 0, GUI_APP_RENDER_RELEASE);
	}

	// delete at the end
	for (int i = endItem + 1; (i < (int) m_vpFolderPictureList->size()); ++i)
	{
		m_pParentControl->SendMessageToParent(0, 0, GUI_APP_RENDER_LOCK);

		std::string filename = m_vpFolderPictureList->at(i);
		SallyAPI::GUI::CPicture* picture = (*m_pmPictureVector)[filename];

		if (picture != NULL)
		{
			(*m_pmPictureVector).erase(filename);
			SafeDelete(picture);
		}

		m_pParentControl->SendMessageToParent(0, 0, GUI_APP_RENDER_RELEASE);
	}

	m_pParentControl->SendMessageToParent(0, 0, GUI_APP_RENDER_LOCK);
	m_pParentControl->SendMessageToParent(0, 0, GUI_APP_FOLDER_PIC_CHANGED);
	m_pParentControl->SendMessageToParent(0, 0, GUI_APP_RENDER_RELEASE);
}