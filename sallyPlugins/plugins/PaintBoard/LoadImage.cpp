////////////////////////////////////////////////////////////////////////////////////////////////////
/// \file	PaintBoard\LoadImage.cpp
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

CLoadImage::CLoadImage()
{
}

CLoadImage::~CLoadImage()
{
}

void CLoadImage::SetValues(SallyAPI::GUI::CForm* control, SallyAPI::GUI::CPicture** picture,
									  const std::string& fileName, int command)
{
	m_ppPicture = picture;
	m_strFileName = fileName;
	m_pParentControl = control;
	m_iCommand = command;
}

void CLoadImage::RunEx()
{
	SallyAPI::GUI::CPicture* newPic = new SallyAPI::GUI::CPicture();
	bool result = newPic->LoadTextureOverGDI(m_strFileName);

	if (!result)
	{
		SafeDelete(newPic);
		return;
	}

	(*m_ppPicture) = newPic;
	m_pParentControl->SendMessageToParent(0, 0, m_iCommand);
}